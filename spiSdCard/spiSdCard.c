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

static uint8_t cardType;			/* b0:MMC, b1:SDv1, b2:SDv2, b3:Block addressing */

void spiSdCardInit(void)
{
    uint8_t data;
    uint16_t i;
    uint8_t buf[4];

    printf("spiSdCardInit()...");
    /*Set up ~CS pin */
    GPIO_PIN_DIR(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_DIR_OUT);
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_HIGH);


    /* Send at least 74 high bits then CMD0 to put card in to SPI mode*/
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_HIGH);
    for(i = 0; i < 10; i++)
        spiTxByte(SPI_SDCARD_SPI_CHAN, 0xff);

    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_LOW);
    //  printf("Send CMD 0\r\n");
    data = spiSdCardSendCommand(SD_CARD_CMD0, 0);
//   printf("SD resp: 0x%.2X\r\n", data);
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_HIGH);

    if(0x01 != data)
    {
        printf("SD INIT FAILED!!\r\n");
        return;
    }


    //  printf("Send CMD 8\r\n");
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_LOW);
    data = spiSdCardSendCommand(SD_CARD_CMD8, 0x000001AA);
    //  printf("SD resp: 0x%.2X\r\n", data);

    if(0x01 != data)
    {
        printf("SD INIT Only V2 cards currently supported!!\r\n");
        return;
    }
    /*Grab rest of CMD8 response*/
    for(i =0; i < 4; i++)
        buf[i] = spiRxByte(SPI_SDCARD_SPI_CHAN);
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_HIGH);

    if(0xAA == buf[3])
        printf("\r\nValid Vdd range: 2.7-3.6V\r\n");
    else
    {
        printf("SD INIT FAILED - Voltage range!!\r\n");
        return;
    }


    /*Send HCS and activatre card init*/
    for(i=0; i < 1000; i++)
    {
        GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_LOW);
        data = spiSdCardSendCommand(SD_CARD_ACMD41, 1 << 30);
        GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_HIGH);

        if(0 == data)
            break;
        delayMs(1);
    }

    if(i > 1000-1)
    {
        printf("SD INIT FAILED - Idle check!!\r\n");
        return;
    }


    /* Check CCS bit in OCR */
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_LOW);
    data = spiSdCardSendCommand(SD_CARD_CMD58, 0);
    for(i =0; i < 4; i++)
        buf[i] = spiRxByte(SPI_SDCARD_SPI_CHAN);
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_HIGH);

    if(buf[0] & 0x40)
    {
        cardType = SD_CARD_SD2;
        printf("Card Type is SD V2\r\n");
    }
    else
    {
        cardType = SD_CARD_BLOCK;
        printf("Card Type is Block\r\n");
    }


    printf("Done.\r\n");
}

uint8_t spiSdCardSendCommand(unsigned char command, uint32_t arg)
{
    uint8_t data = 0xFF;

    if (command & 0x80)  	/* ACMD<n> is the command sequense of CMD55-CMD<n> */
    {
        command &= 0x7F;
        data = spiSdCardSendCommand(SD_CARD_CMD55, 0);
        //     if (data > 1) return data;
        /*Toggle CS to setup for neext command*/
        GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_HIGH);
        GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_LOW);
    }

    spiTxByte(SPI_SDCARD_SPI_CHAN, command);
    spiTxByte(SPI_SDCARD_SPI_CHAN, (arg >> 24) & 0xFF);
    spiTxByte(SPI_SDCARD_SPI_CHAN, (arg >> 16) & 0xFF);
    spiTxByte(SPI_SDCARD_SPI_CHAN, (arg >> 8) & 0xFF);
    spiTxByte(SPI_SDCARD_SPI_CHAN, arg & 0xFF);

    /*Send CRC*/
    /*CRC is only actually checked for CMD8 so we can just send CMD8s CRC for every command*/

    if(SD_CARD_CMD0 == command)
        spiTxByte(SPI_SDCARD_SPI_CHAN, 0x95);
    else if(SD_CARD_CMD8 == command)
        spiTxByte(SPI_SDCARD_SPI_CHAN, 0x87);
    else
        spiTxByte(SPI_SDCARD_SPI_CHAN, 0x01);


    data = 0xFF;
    while(data & 0x80)
        data = spiRxByte(SPI_SDCARD_SPI_CHAN);

    return data;
}

void spiSdCardStoreData(uint32_t address, uint8_t *data, uint16_t dataLen)
{

}

void spiSdCardReadData(uint32_t address, uint8_t *data, uint16_t dataLen)
{

}