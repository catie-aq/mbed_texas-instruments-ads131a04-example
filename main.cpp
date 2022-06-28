/*
 * Copyright (c) 2021, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "ads131a04.h"

using namespace sixtron;

namespace {
#define PERIOD_MS 1000
}

static DigitalOut led1(LED1);
static SPI spi(P1_SPI_MOSI, P1_SPI_MISO, P1_SPI_SCK);

ADS131A04 adc(&spi, P1_SPI_CS, P1_DIO5, P1_DIO1);

EventQueue queue(32 * EVENTS_EVENT_SIZE);

void process_adc(){
    static adc_data_struct adc_data;
    adc.read_adc_data(&adc_data);

    printf("%d\n", adc_data.channel3);   
}

void callback_adc_device(void)
{


    queue.call(process_adc);
}

// main() runs in its own thread in the OS
// (note the calls to ThisThread::sleep_for below for delays)
int main()
{
    // Init ADC
    if (adc.init() == 0) {
        adc.set_gain(ADS131A04::ADC::all, 0x01);

        adc.set_frequency(ADS131A04::Frequency::_500Hz);
        if (adc.start() != 0) {
            printf("Failed to start ADS131A04\n");
        }

    }
    adc.attach_callback(&callback_adc_device);

    queue.dispatch_forever();

    while (true) {
        led1 = !led1;
        if (led1) {
            printf("Alive!\n");
        }
        ThisThread::sleep_for(PERIOD_MS / 2);
    }
}
