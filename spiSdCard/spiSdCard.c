/*
* Copyright 2021 NimoLabs Ltd.
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
* File: spiSdCard.c
* Description: SPI based SD card driver
*/

/*Required defines in nimolib.h
* SPI_SDCARD_CS_PORT
* SPI_SDCARD_CS_PIN
* SPI_SDCARD_SPI_CHAN
*/

#include <sam.h>
#include <nimolib.h>
#include <gpio.h>
#include <spi.h>
#include <delay.h>
#include <printf.h>
#include <spi.h>

#include "spiSdCard.h"

void spiSdCardInit(void)
{
    unsigned char data;
    unsigned char i;
    printf("spiSdCardInit()...");
    /*Set up ~CS pin */
    GPIO_PIN_DIR(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_DIR_OUT);
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_HIGH);


    /* Send at least 74 high bits then CMD0 to put card in to SPI mode*/
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_HIGH);
    for(i = 0; i < 10; i++)
        spiTxByte(SPI_SDCARD_SPI_CHAN, 0xff);

    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_LOW);

    spiSdCardSendCommand(0, 0);

    for(i =0; i < 5; i++)
    {
        data = spiRxByte(SPI_SDCARD_SPI_CHAN);
        printf("SD resp: 0x%.2X\r\n", data);
        if(0x01 == data)
            break;
    }
    if(5 == i)
        printf("SD Card timeout\r\n");
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_HIGH);
    printf("Done.\r\n");
}

void spiSdCardSendCommand(unsigned char command, uint32_t arg)
{
    spiTxByte(SPI_SDCARD_SPI_CHAN, 0x40 | command);
    spiTxByte(SPI_SDCARD_SPI_CHAN, (arg >> 24) & 0xFF);
    spiTxByte(SPI_SDCARD_SPI_CHAN, (arg >> 16) & 0xFF);
    spiTxByte(SPI_SDCARD_SPI_CHAN, (arg >> 8) & 0xFF);
    spiTxByte(SPI_SDCARD_SPI_CHAN, arg & 0xFF);

    /*Send CRC*/
    spiTxByte(SPI_SDCARD_SPI_CHAN, 0x95);
}

void spiSdCardStoreData(uint32_t address, uint8_t *data, uint16_t dataLen)
{

}

void spiSdCardReadData(uint32_t address, uint8_t *data, uint16_t dataLen)
{

}