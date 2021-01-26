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
*
* Inspiration: http://elm-chan.org/fsw/ff/pfsample.zip
*
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
        cardType = SD_CARD_CT_SD2;
        printf("Card Type is SD V2\r\n");
    }
    else
    {
        cardType = SD_CARD_CT_BLOCK;
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

/*-----------------------------------------------------------------------*/
/* Read partial sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT spiSdCardReadData (
    uint8_t *buff,		/* Pointer to the read buffer (NULL:Read bytes are forwarded to the stream) */
    uint32_t sector,	/* Sector number (LBA) */
    uint16_t offset,	/* Byte offset to read from (0..511) */
    uint16_t count		/* Number of bytes to read (ofs + cnt mus be <= 512) */
)
{
    DRESULT res;
    uint8_t d;
    uint16_t bc, tmr;
    uint16_t i;


    if (!(cardType & SD_CARD_CT_BLOCK)) sector *= 512;	/* Convert to byte address if needed */

    res = RES_ERROR;
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_LOW);
    if (spiSdCardSendCommand(SD_CARD_CMD17, sector) == 0)  		/* READ_SINGLE_BLOCK */
    {
        printf("Read succeed\r\n");
        tmr = 1000;
        do  							/* Wait for data packet in timeout of 100ms */
        {
            delayMs(1); /*Might get away with 0.1mS?*/
            d = spiRxByte(SPI_SDCARD_SPI_CHAN);
        }
        while (d == 0xFF && --tmr);

        if (d == 0xFE)  				/* A data packet arrived */
        {
            printf("Found FE\r\n");
            bc = 514 - offset - count;

            /* Skip leading bytes */
            if (offset)
                for(i=0; i < offset; i++)
                    spiRxByte(SPI_SDCARD_SPI_CHAN);

            /* Receive a part of the sector */
            if (buff)  	/* Store data to the memory */
            {
                do
                    *buff++ = spiRxByte(SPI_SDCARD_SPI_CHAN);
                while (--count);
            }

            /* Skip trailing bytes and CRC */
            for(i=0; i < bc; i++)
                spiRxByte(SPI_SDCARD_SPI_CHAN);

            res = RES_OK;
        }
    }
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_HIGH);


    return res;
}



/*-----------------------------------------------------------------------*/
/* Write partial sector                                                  */
/*-----------------------------------------------------------------------*/
DRESULT spiSdCardStoreData (
    const uint8_t *buff,	/* Pointer to the bytes to be written (NULL:Initiate/Finalize sector write) */
    uint32_t sc			/* Number of bytes to send, Sector number (LBA) or zero */
)
{
    DRESULT res;
    uint16_t bc, tmr;
    static uint16_t wc;


    res = RES_ERROR;

    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_LOW);

    if (buff)  		/* Send data bytes */
    {
        bc = (uint16_t)sc;
        while (bc && wc)  		/* Send data bytes to the card */
        {
            spiTxByte(SPI_SDCARD_SPI_CHAN,*buff++);
            wc--;
            bc--;
        }
        res = RES_OK;
    }
    else
    {
        if (sc)  	/* Initiate sector write transaction */
        {
            if (!(cardType & SD_CARD_CT_BLOCK)) sc *= 512;	/* Convert to byte address if needed */
            if (spiSdCardSendCommand(SD_CARD_CMD24, sc) == 0)  			/* WRITE_SINGLE_BLOCK */
            {
                spiTxByte(SPI_SDCARD_SPI_CHAN,0xFF);
                spiTxByte(SPI_SDCARD_SPI_CHAN,0xFE);		/* Data block header */
                wc = 512;							/* Set byte counter */
                res = RES_OK;
            }
        }
        else  	/* Finalize sector write transaction */
        {
            bc = wc + 2;
            while (bc--) spiTxByte(SPI_SDCARD_SPI_CHAN,0);	/* Fill left bytes and CRC with zeros */
            if ((spiRxByte(SPI_SDCARD_SPI_CHAN) & 0x1F) == 0x05)  	/* Receive data resp and wait for end of write process in timeout of 300ms */
            {
                for (tmr = 10000; spiRxByte(SPI_SDCARD_SPI_CHAN) != 0xFF && tmr; tmr--)	/* Wait for ready (max 1000ms) */
                    delayMs(1); /*Might get away with 0.1mS?*/
                if (tmr) res = RES_OK;
            }
        }
    }
    GPIO_PIN_OUT(SPI_SDCARD_CS_PORT, SPI_SDCARD_CS_PIN, GPIO_OUT_HIGH);
    return res;
}