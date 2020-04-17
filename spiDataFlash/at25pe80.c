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
#include <spi.h>
#include <printf.h>
#include <delay.h>

#include "at25pe80.h"

void spiDataFlashInit(unsigned char chip __attribute__((unused)))
{
    printf("spiDataFlashInit()...");
    SPI_DATAFLASH_AT25PE80_CS_SETUP;
    SPI_DATAFLASH_AT25PE80_CS_DESELECT;
    printf("Done.\r\n");
}
void spiDataFlashReadData(unsigned char chip __attribute__((unused)), unsigned long address,
                          unsigned char *data, unsigned int dataLen)
{
    SPI_DATAFLASH_AT25PE80_CS_SELECT;
    spiTxByte(AT25PE80_CONTINUOUS_READ_SLOW);
    spiTxByte((address >> 16) & 0xff);
    spiTxByte((address >> 8) & 0xff);
    spiTxByte(address & 0xff);

    for (unsigned int i = 0; i < dataLen; i++)
        data[i] = spiRxByte();
    SPI_DATAFLASH_AT25PE80_CS_DESELECT;
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

    for (unsigned int i = 0; i < iter; i++)
    {
        SPI_DATAFLASH_AT25PE80_CS_SELECT;
        spiTxByte(AT25PE80_PROG_RMW);
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
        SPI_DATAFLASH_AT25PE80_CS_DESELECT;

        delayMs(55);
        address += dataLen;
    }
}

void spiDataFlashChipErase(unsigned char chip __attribute__((unused)))
{
    SPI_DATAFLASH_AT25PE80_CS_SELECT;
    spiTxByte(0xC7);
    spiTxByte(0x94);
    spiTxByte(0x80);
    spiTxByte(0x9A);
    SPI_DATAFLASH_AT25PE80_CS_DESELECT;
    delayMs(20000);
}