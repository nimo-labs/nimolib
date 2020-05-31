/*
* Copyright 2020 NimoLabs Ltd.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* File: samSpiBB.c
* Description: ATSAMD21 bit bashed SPI device driver
*/

/* hal.h requirements:
*BBSPI_CHAN0_MOSI_SETUP
*BBSPI_CHAN0_MOSI_DESELECT
*BBSPI_CHAN0_SCK_SETUP
*BBSPI_CHAN0_SCK_DESELECT
*BBSPI_CHAN0_MISO_SETUP
*/

#include <sam.h>
#include <system.h>
#include <nimolib.h>
#include <delay.h>
#include <gpio.h>
#include <printf.h>

#include "spi.h"

static unsigned char rxByte;

void spiInit(void)
{
    printf("spiInit() (BB)...");

    /*Set MOSI and SCK as outputs*/
    GPIO_PIN_DIR(BBSPI_CHAN0_MOSI_PORT, BBSPI_CHAN0_MOSI_PIN, GPIO_DIR_OUT);
    GPIO_PIN_OUT(BBSPI_CHAN0_MOSI_PORT, BBSPI_CHAN0_MOSI_PIN, GPIO_OUT_LOW);
    GPIO_PIN_DIR(BBSPI_CHAN0_MISO_PORT, BBSPI_CHAN0_MISO_PIN, GPIO_DIR_IN);
    GPIO_PIN_DIR(BBSPI_CHAN0_SCK_PORT, BBSPI_CHAN0_SCK_PIN, GPIO_DIR_OUT);
    GPIO_PIN_OUT(BBSPI_CHAN0_SCK_PORT, BBSPI_CHAN0_SCK_PIN, GPIO_OUT_LOW);

    printf("Done.\r\n");
}

static void bitDelay(void)
{
    unsigned long i;
#warning this should be using a better delay routine
    for (i = 0; i < 400; i++)
        asm("nop");
}

void spiTxByte(unsigned char byte)
{
    unsigned char i;
    rxByte = 0;
    for (i = 0; i < 8; i++)
    {
        if (byte & (0x01 << (7 - i)))
            GPIO_PIN_OUT(BBSPI_CHAN0_MOSI_PORT, BBSPI_CHAN0_MOSI_PIN, GPIO_OUT_HIGH);
        else
            GPIO_PIN_OUT(BBSPI_CHAN0_MOSI_PORT, BBSPI_CHAN0_MOSI_PIN, GPIO_OUT_LOW);

        if ((GPIO_PIN_READ(BBSPI_CHAN0_MISO_PORT, BBSPI_CHAN0_MISO_PIN)) > 0)
            rxByte |= (1 << 0);
        else
            rxByte &= ~(1 << 0);
        if (i < 7)
            rxByte <<= 1;

        GPIO_PIN_OUT(BBSPI_CHAN0_SCK_PORT, BBSPI_CHAN0_SCK_PIN, GPIO_OUT_HIGH);
        bitDelay();
        GPIO_PIN_OUT(BBSPI_CHAN0_SCK_PORT, BBSPI_CHAN0_SCK_PIN, GPIO_OUT_LOW);
        bitDelay();
    }
}

unsigned char spiRxByte(void)
{
    spiTxByte(0x00); /*Write dummy byte*/
    return rxByte;
}