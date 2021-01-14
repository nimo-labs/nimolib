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

void spiSdCardInit(void);
void spiSdCardStoreData(uint32_t address, uint8_t *data, uint16_t dataLen);
void spiSdCardReadData(uint32_t address, uint8_t *data, uint16_t dataLen);
#endif