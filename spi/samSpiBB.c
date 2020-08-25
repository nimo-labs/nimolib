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

void spiInit(unsigned char channel)
{
    printf("spiInit() (BB)...");

    /*Set MOSI and SCK as outputs*/
#ifdef SPIBB_USE_CHAN0
    if (SPI_CHAN0 == channel)
    {
        GPIO_PIN_DIR(BBSPI_CHAN0_MOSI_PORT, BBSPI_CHAN0_MOSI_PIN, GPIO_DIR_OUT);
        GPIO_PIN_OUT(BBSPI_CHAN0_MOSI_PORT, BBSPI_CHAN0_MOSI_PIN, GPIO_OUT_LOW);
        GPIO_PIN_DIR(BBSPI_CHAN0_MISO_PORT, BBSPI_CHAN0_MISO_PIN, GPIO_DIR_IN);
        GPIO_PIN_DIR(BBSPI_CHAN0_SCK_PORT, BBSPI_CHAN0_SCK_PIN, GPIO_DIR_OUT);
        GPIO_PIN_OUT(BBSPI_CHAN0_SCK_PORT, BBSPI_CHAN0_SCK_PIN, GPIO_OUT_LOW);
    }
#endif
#ifdef SPIBB_USE_CHAN1
    if (SPI_CHAN1 == channel)
    {
        GPIO_PIN_DIR(BBSPI_CHAN1_MOSI_PORT, BBSPI_CHAN1_MOSI_PIN, GPIO_DIR_OUT);
        GPIO_PIN_OUT(BBSPI_CHAN1_MOSI_PORT, BBSPI_CHAN1_MOSI_PIN, GPIO_OUT_LOW);
        GPIO_PIN_DIR(BBSPI_CHAN1_MISO_PORT, BBSPI_CHAN1_MISO_PIN, GPIO_DIR_IN);
        GPIO_PIN_DIR(BBSPI_CHAN1_SCK_PORT, BBSPI_CHAN1_SCK_PIN, GPIO_DIR_OUT);
        GPIO_PIN_OUT(BBSPI_CHAN1_SCK_PORT, BBSPI_CHAN1_SCK_PIN, GPIO_OUT_LOW);
    }
#endif
    printf("Done.\r\n");
}

static void bitDelay(void)
{
    unsigned long i;
#warning this should be using a better delay routine
    for (i = 0; i < 600; i++)
        asm("nop");
}

void spiTxByte(unsigned char channel, unsigned char byte)
{
    unsigned char i;
    rxByte = 0;
    for (i = 0; i < 8; i++)
    {
        if (byte & (0x01 << (7 - i)))
        {
#ifdef SPIBB_USE_CHAN0
            if (SPI_CHAN0 == channel)
                GPIO_PIN_OUT(BBSPI_CHAN0_MOSI_PORT, BBSPI_CHAN0_MOSI_PIN, GPIO_OUT_HIGH);
#endif
#ifdef SPIBB_USE_CHAN1
            if (SPI_CHAN1 == channel)
                GPIO_PIN_OUT(BBSPI_CHAN1_MOSI_PORT, BBSPI_CHAN1_MOSI_PIN, GPIO_OUT_HIGH);
#endif
        }
        else
        {
#ifdef SPIBB_USE_CHAN0
            if (SPI_CHAN0 == channel)
                GPIO_PIN_OUT(BBSPI_CHAN0_MOSI_PORT, BBSPI_CHAN0_MOSI_PIN, GPIO_OUT_LOW);
#endif
#ifdef SPIBB_USE_CHAN1
            if (SPI_CHAN1 == channel)
                GPIO_PIN_OUT(BBSPI_CHAN1_MOSI_PORT, BBSPI_CHAN1_MOSI_PIN, GPIO_OUT_LOW);
#endif
        }
#ifdef SPIBB_USE_CHAN0
        if (SPI_CHAN0 == channel)
        {
            if ((GPIO_PIN_READ(BBSPI_CHAN0_MISO_PORT, BBSPI_CHAN0_MISO_PIN)) > 0)
                rxByte |= (1 << 0);
            else
                rxByte &= ~(1 << 0);
            if (i < 7)
                rxByte <<= 1;
        }
#endif
#ifdef SPIBB_USE_CHAN1
        if (SPI_CHAN1 == channel)
        {
            if ((GPIO_PIN_READ(BBSPI_CHAN1_MISO_PORT, BBSPI_CHAN1_MISO_PIN)) > 0)
                rxByte |= (1 << 0);
            else
                rxByte &= ~(1 << 0);
            if (i < 7)
                rxByte <<= 1;
        }
#endif

#ifdef SPIBB_USE_CHAN0
        if (SPI_CHAN0 == channel)
            GPIO_PIN_OUT(BBSPI_CHAN0_SCK_PORT, BBSPI_CHAN0_SCK_PIN, GPIO_OUT_HIGH);
#endif
#ifdef SPIBB_USE_CHAN1
        if (SPI_CHAN1 == channel)
            GPIO_PIN_OUT(BBSPI_CHAN1_SCK_PORT, BBSPI_CHAN1_SCK_PIN, GPIO_OUT_HIGH);
#endif

        bitDelay();
#ifdef SPIBB_USE_CHAN0
        if (SPI_CHAN0 == channel)
            GPIO_PIN_OUT(BBSPI_CHAN0_SCK_PORT, BBSPI_CHAN0_SCK_PIN, GPIO_OUT_LOW);
#endif
#ifdef SPIBB_USE_CHAN1
        if (SPI_CHAN1 == channel)
            GPIO_PIN_OUT(BBSPI_CHAN1_SCK_PORT, BBSPI_CHAN1_SCK_PIN, GPIO_OUT_LOW);
#endif
        bitDelay();
    }
}

unsigned char spiRxByte(unsigned char channel)
{
    spiTxByte(channel, 0x00); /*Write dummy byte*/
    return rxByte;
}