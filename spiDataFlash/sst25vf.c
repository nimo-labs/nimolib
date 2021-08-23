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
#include <gpio.h>
#include <spi.h>
#include <printf.h>
#include <delay.h>

#include "sst25vf.h"

static void bitDelay(void)
{
    unsigned long i;
#warning this should be using a better delay routine
    for (i = 0; i < 600; i++)
        asm("nop");
}

void spiDataFlashInit(unsigned char chip __attribute__((unused)))
{
    printf("spiDataFlashInit()...");
    GPIO_PIN_DIR(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_DIR_OUT);
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);

    /*Enable status write*/
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, SST25VF_CMD_EN_WRITE_STATUS_REG);
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);
    delayMs(10);

    /*Disable write protection for whole chip*/
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, SST25VF_CMD_WRITE_STATUS_REG);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, 0x00); /*Disable all write protection */
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);
    delayMs(10);

    printf("Done.\r\n");
}
void spiDataFlashReadData(unsigned char chip __attribute__((unused)), unsigned long address,
                          unsigned char *data, unsigned int dataLen)
{
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, SST25VF_CMD_READ);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, (address >> 16) & 0xff);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, (address >> 8) & 0xff);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, address & 0xff);

    for (unsigned int i = 0; i < dataLen; i++)
        data[i] = spiRxByte(SPI_DATAFLASH_SPI_CHAN);
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);
}

static void writeByte(unsigned long address, unsigned char data)
{
    /*Perform write enable*/
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, SST25VF_CMD_WRITE_EN);
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);
    delayMs(1);
    /*Write data*/

    /*Write first byte with address*/
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, SST25VF_CMD_BYTE_PROGRAM);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, (address >> 16) & 0xff);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, (address >> 8) & 0xff);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, address & 0xff);

    spiTxByte(SPI_DATAFLASH_SPI_CHAN, data);
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);

    //delayMs(1);
    bitDelay();
}

void spiDataFlashPageWrite(unsigned char chip __attribute__((unused)), unsigned long address,
                           unsigned char *data, unsigned char dataLen)
{
    for (unsigned int i = 0; i < dataLen; i++)
        writeByte(address++, data[i]);
}

void spiDataFlash4kErase(unsigned char chip __attribute__((unused)), unsigned long address)
{
    /*Perform write enable*/
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, SST25VF_CMD_WRITE_EN);
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);
    delayMs(1);

    /*Write erase command with address*/
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, SST25VF_CMD_4K_ERASE);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, (address >> 16) & 0xff);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, (address >> 8) & 0xff);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, address & 0xff);

    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);

    delayMs(30);
}
void spiDataFlashChipErase(unsigned char chip __attribute__((unused)))
{
    /*Perform write enable*/
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, SST25VF_CMD_WRITE_EN);
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);
    delayMs(1);

    /*Write erase command*/
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(SPI_DATAFLASH_SPI_CHAN, SST25VF_CMD_CHIP_ERASE);
    GPIO_PIN_OUT(SPI_DATAFLASH_CS_PORT, SPI_DATAFLASH_CS_PIN, GPIO_OUT_HIGH);

    delayMs(6000);
}