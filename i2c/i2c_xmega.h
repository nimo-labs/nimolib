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
* File: atsamd21.c
* Description: ATSAMD21 SPI device driver
*/

#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
extern "C"
{
#endif

#define I2C_BAUD_100K 0
#define I2C_BAUD_400K 1

  void i2cInit(unsigned int baudRate);
  void i2cWrite(unsigned char addr, unsigned char subaddr, unsigned char value);
  void i2cWrite16(unsigned char addr, unsigned int subaddr, unsigned int value);
  unsigned char i2cRead(unsigned char addr, unsigned char subaddr);
  unsigned int i2cRead16(unsigned char addr, unsigned int subaddr);
  void i2cPing(unsigned char addr);
  unsigned int i2cNoAddrpwRead16(unsigned char addr);
  void i2cWriteGeneric(unsigned char addr, unsigned char *data, unsigned char dataLen);
  unsigned char i2cReadEeprom(unsigned char addr, unsigned int subaddr);
  unsigned int i2cReadAd7997(unsigned char addr, unsigned char commandByte);

#ifdef __cplusplus
}
#endif

#endif /* I2C_H */
