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
* File: 24aa08.c
* Description: Micorchip 24AA08 I2C EEPROM
*/
/**
 * The driver requires the following I/O defines to be present in the projects system.h
 * \code
 * #define  BB_SPI_CHAN0
 *
 * #define AT86RF23X_CS_PORT SAM_GPIO_PORTB
 * #define AT86RF23X_CS_PIN 2
 *
 *  #define AT86RF23X_SLP_TR_PORT SAM_GPIO_PORTB
 * #define AT86RF23X_SLP_TR_PIN 1
 *
 * #define AT86RF23X_RESET_PORT SAM_GPIO_PORTB
 * #define AT86RF23X_RESET_PIN 0
 * \endcode
 *
 *Basic setup of the chip involves the following:
 *\code
 * at86rf23xInit();
 * at86rf23xSetChannel(radioChan);
 * at86rf23xSetPanId(1);
 * at86rf23xSetAddr(1);
 *\endcode
 */

#include <sam.h>
#include <nimolib.h>
#include <i2c.h>
#include <delay.h>
#include <printf.h>

#include "24aa08.h"

void m24aa08Init(void)
{
    printf("24AA08Init()...");

    printf("Done.\r\n");
}

static ackCheck(void)
{
    unsigned char ret;
    for (int i = 0; i < 10; i++)
    {
        if(!i2cRead(M24AA08_ADDR_BASE, &ret, 1))
            break;
    }
}

unsigned char m24aa08WriteByte(unsigned char memBank, unsigned char addr, unsigned char data)
{
    unsigned char pkt[2];
    pkt[0] = addr;
    pkt[1] = data;
    i2cWrite(M24AA08_ADDR_BASE + memBank, pkt, 2, 1);
    ackCheck();
}

void m24aa08WriteBytes(unsigned char memBank, unsigned char addr, unsigned char *data, unsigned char dataLen)
{
    unsigned char lBuf[17];
    /*Data writes can't exceed 16 bytes of data!*/

    lBuf[0] = addr;
    lBuf[1] = 0x01;
    lBuf[2] = 0x02;
    lBuf[3] = 0x03;
    lBuf[4] = 0x04;

    // for(char i=0; i < dataLen; i++)
    //     lBuf[i+1] = data[i];

    if(((unsigned int) addr) + dataLen < 256)
    {
        i2cWrite(M24AA08_ADDR_BASE + memBank, lBuf, dataLen+1, 1); /*Write start address*/
        ackCheck();
    }
    else
        printf("m24aa08WriteBytes() data exceeds boundary\r\n");
}

unsigned char m24aa08ReadByte(unsigned char memBank, unsigned char addr)
{
    unsigned char ret;
    i2cWrite(M24AA08_ADDR_BASE+memBank, &addr, 1,0);
    i2cRead(M24AA08_ADDR_BASE+memBank, &ret, 1);

    return ret;
}

void m24aa08ReadBytes(unsigned char memBank, unsigned char addr, unsigned char * data, unsigned char dataLen)
{
    i2cWrite(M24AA08_ADDR_BASE+memBank, &addr, 1,0);
    i2cRead(M24AA08_ADDR_BASE+memBank, data, dataLen);
}

