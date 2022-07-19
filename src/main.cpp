/*
 * Copyright (c) 2022, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "Biquad.h"
#include "ads131a04.h"
#include "mbed.h"

#define PRINTF_PERIOD 10ms
#define MAX_MEASURES 400

#define STEP_0_5V (5.0f / float(0x01000000))
#define STEP_0_10V (10.0f / float(0x01000000))
#define STEP_4_20MA ((0.025f / float(0x01000000)) * 1000.0f)

#define RAW_TO_0_5V(raw) (float(raw + 0x00800000) * STEP_0_5V)
#define RAW_TO_0_10V(raw) (float(raw + 0x00800000) * STEP_0_10V)
#define RAW_TO_4_20MA(raw) (float(raw + 0x00800000) * STEP_4_20MA)

// Main Debug, just for the example. See main loop.
#define MAIN_LOOP_FLAG 0x01
Ticker mainLoopTicker;
EventFlags mainLoopFlag;

#define ADC_FLAG 0x02
#define ADC_PERIOD 10ms
Ticker adcTicker;
EventFlags adcFlag;
Thread adcThread(osPriorityRealtime1);

static Biquad channel1_filter, channel2_filter, channel3_filter, channel4_filter;
static float channel1_f[MAX_MEASURES] ;//, channel2_f, channel3_f, channel4_f;

// Set up printf over FTDI
static UnbufferedSerial terminal(CONSOLE_TX,
        CONSOLE_RX,
        115200); // (921600) DO NOT USE BUFFERED WHEN PLAYING WITH THREAD

FileHandle *mbed::mbed_override_console(int fd)
{
    return &terminal;
}

using namespace sixtron;
static SPI spi_ads(SPI1_MOSI, SPI1_MISO, SPI1_SCK);
static ADS131A04 sensor_4_20ma(&spi_ads, SPI1_CS, DIO5, DIO1);
static adc_data_struct datas[MAX_MEASURES];

static DigitalOut led1(LED1);

void mainLoopUpdate()
{
    mainLoopFlag.set(MAIN_LOOP_FLAG);
}

void adcUpdateFlag()
{
    adcFlag.set(ADC_FLAG);
}

void adcUpdate()
{

//    auto f_secs = std::chrono::duration_cast<std::chrono::duration<float>>(ADC_PERIOD);
//    float dt_adc = f_secs.count();
    float adc_rate = 2666.0f; //hz

    channel1_filter.setBiquad(bq_type_lowpass, 50.0f / adc_rate, 0.707f, 0.0f);
    //    channel2_filter.setBiquad(bq_type_lowpass, 20.0f * dt_adc, 0.707f, 0.0f);
    //    channel3_filter.setBiquad(bq_type_lowpass, 5.0f * dt_adc, 0.707f, 0.0f);
    //    channel4_filter.setBiquad(bq_type_lowpass, 5.0f * dt_adc, 0.707f, 0.0f);

    int i = 0;
    bool continue_reading_adc = true;
    while (continue_reading_adc) {
        // Wait for trig
        adcFlag.wait_any(ADC_FLAG);

        sensor_4_20ma.read_adc_data(&datas[i]);

        if (i >= MAX_MEASURES) {
            continue_reading_adc = false;
            led1 = 1;
        }

        channel1_f[i] = (channel1_filter.process(RAW_TO_0_5V(datas[i].channel1)));

        //        channel2_f = (channel2_filter.process(RAW_TO_0_10V(datas.channel2)));
        //        channel3_f = (channel3_filter.process(RAW_TO_4_20MA(datas.channel3)));
        //        channel4_f = (channel4_filter.process(RAW_TO_4_20MA(datas.channel4)));

        i++;
    }
}

int main()
{

    led1 = 1;

    printf("Init...\n");
    ThisThread::sleep_for(3000ms);

    led1 = 0;
    // Init ADC
    if (sensor_4_20ma.init() != 0) {
        printf("Failed to init ADS131A04\n");
    } else {
        sensor_4_20ma.set_gain(ADS131A04::ADC::all, 0x01);
        sensor_4_20ma.set_frequency(ADS131A04::Frequency::_2666Hz);
        if (sensor_4_20ma.start() != 0) {
            printf("Failed to start ADS131A04\n");
        } else {
            printf("ADS131A04 setup done.\n");
        }
    }

    printf("Setup end.\n");
    ThisThread::sleep_for(500ms);

    auto f_secs = std::chrono::duration_cast<std::chrono::duration<float>>(500us);
    float time_incr = f_secs.count();

    mainLoopTicker.attach(&mainLoopUpdate, PRINTF_PERIOD);

    adcThread.start(adcUpdate);
    //    adcTicker.attach(&adcUpdateFlag, ADC_PERIOD);

    sensor_4_20ma.attach_callback(adcUpdateFlag);

    char buffer[100];
    int size;
    float time = 0;

    printf("Data ready. Press key to send through Serial.\n");

    while (!terminal.readable());

    for (int j = 0; j < MAX_MEASURES; j++) {
        mainLoopFlag.wait_any(MAIN_LOOP_FLAG);

        size = sprintf(buffer,
                "%6.4f;%8ld;%8ld;%8ld;%8ld;%9.6f;%9.6f;%9.6f;%9.6f;\n",
                time,
                datas[j].channel1,
                datas[j].channel2,
                datas[j].channel3,
                datas[j].channel4,
                RAW_TO_0_5V(datas[j].channel1),
                channel1_f[j],
                RAW_TO_4_20MA(datas[j].channel3),
                RAW_TO_4_20MA(datas[j].channel4));
        terminal.write(&buffer, size);

        time += time_incr;
    }

    led1 = 0;

    printf("data send\n.");

    while (true) {
        mainLoopFlag.wait_any(MAIN_LOOP_FLAG);

        //        led1 = !led1;

        //        size = sprintf(buffer,
        //                "t:%6.2fs  IN1: %8ld    IN2: %8ld    IN3: %8ld     IN4: %8ld\n",
        //                time,
        //                datas.channel1,
        //                datas.channel2,
        //                datas.channel3,
        //                datas.channel4);
        //        terminal.write(&buffer, size);

        //        size = sprintf(buffer,
        //                "t:%6.2fs  IN1: %9.6fV  IN2: %9.6ffV  IN3: %9.6fmA  IN4: %9.6fmA\n",
        //                time,
        //                channel1_f,
        //                channel2_f,
        //                channel3_f,
        //                channel4_f);
        //        terminal.write(&buffer, size);

        //        size = sprintf(buffer,
        //                "t:%6.2fs  IN1: %9.6fV  IN2: %9.6fV  IN3: %9.6fmA  IN4: %9.6fmA\n",
        //                time,
        //                RAW_TO_0_5V(datas.channel1),
        //                RAW_TO_0_10V(datas.channel2),
        //                RAW_TO_4_20MA(datas.channel3),
        //                RAW_TO_4_20MA(datas.channel4));
        //        terminal.write(&buffer, size);

        time += time_incr;
    }
}
