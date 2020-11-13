/**
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

/* void i2cInit(unsigned char chan, unsigned int baudRate); */
/* unsigned char i2cWrite(unsigned char chan, unsigned char address, unsigned char *data, unsigned char len, unsigned char stop); */
/* unsigned char i2cRead(unsigned char chan, unsigned char address, unsigned char * data, unsigned char len); */

void i2cInit(unsigned int baudRate);
unsigned char i2cWrite(unsigned char address, unsigned char *data,
                       unsigned int len, unsigned char stop);
unsigned char i2cRead(unsigned char address, unsigned char *data,
                      unsigned int len);

unsigned char i2cPing(unsigned char chan, unsigned char addr);
void i2cDetect(__attribute__((unused)) unsigned char (chan));

#if defined(__SAMD21)
#include "i2c_samd.h"
#elif defined(__SAMD21)
#include "i2c_samd.h"
#else
#include "i2c_xmega.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /* I2C_H */
