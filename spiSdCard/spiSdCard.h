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
* File: at86rf23x.h
* Description: AT86RF23X / 2.4GHz transceiver driver
*/
#ifndef at86rf23xH
#define at86rf23xH

#define SD_CARD_CMD0 0x40 + 0
#define SD_CARD_CMD8 0x40 + 8
#define SD_CARD_CMD16 0x40 + 16
#define SD_CARD_CMD17 0x40 + 17
#define SD_CARD_CMD24 0x40 + 24
#define SD_CARD_ACMD41 0xC0 + 41
#define SD_CARD_CMD55 0x40 + 55
#define SD_CARD_CMD58 0x40 + 58

/* Card type flags (CardType) */
#define SD_CARD_CT_MMC				0x01	/* MMC ver 3 */
#define SD_CARD_CT_SD1				0x02	/* SD ver 1 */
#define SD_CARD_CT_SD2				0x04	/* SD ver 2 */
#define SD_CARD_CT_SDC				(SD_CARD_CT_SD1|SD_CARD_CT_SD2)	/* SD */
#define SD_CARD_CT_BLOCK			0x08	/* Block addressing */

/* Results of Disk Functions */
typedef enum
{
    RES_OK = 0,		/* 0: Function succeeded */
    RES_ERROR,		/* 1: Disk error */
    RES_NOTRDY,		/* 2: Not ready */
    RES_PARERR		/* 3: Invalid parameter */
} DRESULT;

void spiSdCardInit(void);
uint8_t spiSdCardSendCommand(unsigned char command, uint32_t arg);
DRESULT spiSdCardReadData (
    uint8_t *buff,		/* Pointer to the read buffer (NULL:Read bytes are forwarded to the stream) */
    uint32_t sector,	/* Sector number (LBA) */
    uint16_t offset,	/* Byte offset to read from (0..511) */
    uint16_t count		/* Number of bytes to read (ofs + cnt mus be <= 512) */
);
DRESULT spiSdCardStoreData (
    const uint8_t *buff,	/* Pointer to the bytes to be written (NULL:Initiate/Finalize sector write) */
    uint32_t sc			/* Number of bytes to send, Sector number (LBA) or zero */
);
#endif