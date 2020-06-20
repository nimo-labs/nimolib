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
* File: spi.h
* Description: ATSAMD21 SPI API provider
*/

#define SPI_CHAN0 0
#define SPI_CHAN1 1

void spiInit(unsigned char channel);
void spiTxByte(unsigned char channel, unsigned char byte);
unsigned char spiRxByte(unsigned char channel);
