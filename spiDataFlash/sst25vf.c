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
* File: at25pe80.c
* Description: AT25PE80 dataflash chip device driver
*/
#include <sam.h>
#include <nimolib.h>
#include <gpio.h>
#include <spi.h>
#include <printf.h>
#include <delay.h>

#include "sst25vf.h"

void spiDataFlashInit(unsigned char chip __attribute__((unused)))
{
    printf("spiDataFlashInit()...");
    GPIO_PIN_DIR(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_DIR_OUT);
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);
    printf("Done.\r\n");
}
void spiDataFlashReadData(unsigned char chip __attribute__((unused)), unsigned long address,
                          unsigned char *data, unsigned int dataLen)
{
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(SST25VF_CMD_READ);
    spiTxByte((address >> 16) & 0xff);
    spiTxByte((address >> 8) & 0xff);
    spiTxByte(address & 0xff);

    for (unsigned int i = 0; i < dataLen; i++)
        data[i] = spiRxByte();
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);
}

void spiDataFlashPageWrite(unsigned char chip __attribute__((unused)), unsigned long address,
                           unsigned char *data, unsigned char dataLen)
{
    unsigned long totalLen = address + dataLen;
    unsigned int iter;
    unsigned int dataCtr = 0;

    if (((address & 0xff) + dataLen) < 255)
        iter = 1;
    else
        iter = (((address & 0xff) + dataLen) / 255) + 1;

    /*Perform write enable*/
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(SST25VF_CMD_WRITE_EN);
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);

    /*Write data*/
    for (unsigned int i = 0; i < iter; i++)
    {
        GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
        spiTxByte(SST25VF_CMD_BYTE_PROGRAM);
        spiTxByte((address >> 16) & 0xff);
        spiTxByte((address >> 8) & 0xff);
        spiTxByte(address & 0xff);

        if (i < (iter - 1))
            dataLen = 256 - (address & 0xff);
        else
            dataLen = totalLen - address;
        if (dataLen == ((totalLen - address) + 1))
            dataLen--;

        for (unsigned int j = 0; j < dataLen; j++, dataCtr++)
            spiTxByte(data[dataCtr]);
        GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);

        delayMs(55);
        address += dataLen;
    }
}

void spiDataFlashChipErase(unsigned char chip __attribute__((unused)))
{
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(0xC7);
    spiTxByte(0x94);
    spiTxByte(0x80);
    spiTxByte(0x9A);
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);
    delayMs(20000);
}