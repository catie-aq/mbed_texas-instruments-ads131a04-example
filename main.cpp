/*
 * Copyright (c) 2021, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

using namespace sixtron;

namespace {
#define PERIOD_MS 1000
}

static DigitalOut led1(LED1);

// main() runs in its own thread in the OS
// (note the calls to ThisThread::sleep_for below for delays)
int main()
{
    while (true) {
        led1 = !led1;
        if (led1) {
            printf("Alive!\n");
        }
        ThisThread::sleep_for(PERIOD_MS / 2);
    }
}
