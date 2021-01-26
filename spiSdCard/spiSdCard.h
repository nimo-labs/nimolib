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
#define SD_CARD_ACMD41 0xC0 + 41
#define SD_CARD_CMD55 0x40 + 55
#define SD_CARD_CMD58 0x40 + 58

/* Card type flags (CardType) */
#define SD_CARD_MMC				0x01	/* MMC ver 3 */
#define SD_CARD_SD1				0x02	/* SD ver 1 */
#define SD_CARD_SD2				0x04	/* SD ver 2 */
#define SD_CARD_SDC				(SD_CARD_SD1|SD_CARD_SD2)	/* SD */
#define SD_CARD_BLOCK			0x08	/* Block addressing */

void spiSdCardInit(void);
uint8_t spiSdCardSendCommand(unsigned char command, uint32_t arg);
void spiSdCardStoreData(uint32_t address, uint8_t *data, uint16_t dataLen);
void spiSdCardReadData(uint32_t address, uint8_t *data, uint16_t dataLen);
#endif