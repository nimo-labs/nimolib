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
* File: 24aa08.h
* Description: Micorchip 24AA08 I2C EEPROM
*/
#ifndef M24AA08H
#define M24AA08H

#define M24AA08_ADDR_BASE 0x50

#define M24AA08_BANK0 0x00
#define M24AA08_BANK1 0x01
#define M24AA08_BANK2 0x02
#define M24AA08_BANK3 0x03

void m24aa08Init(void);
unsigned char m24aa08WriteByte(unsigned char memBank, unsigned char addr, unsigned char data);
void m24aa08WriteBytes(unsigned char memBank, unsigned char addr, unsigned char *data, unsigned char dataLen);
unsigned char m24aa08ReadByte(unsigned char memBank, unsigned char addr);
void m24aa08ReadBytes(unsigned char memBank, unsigned char addr, unsigned char * data, unsigned char dataLen);

#endif // M24AA08H