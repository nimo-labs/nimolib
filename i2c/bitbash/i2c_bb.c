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

#include <sam.h>

#define SDA_SETUP                                                   \
    PORT->Group[I2C_BB_SDA_PORT].OUT.reg &= ~(1 << I2C_BB_SDA_PIN); \
    PORT->Group[I2C_BB_SDA_PORT].PINCFG[I2C_BB_SDA_PIN].reg |= PORT_PINCFG_INEN
#define SDA_SELECT PORT->Group[I2C_BB_SDA_PORT].DIR.reg &= ~(1 << I2C_BB_SDA_PIN)
#define SDA_DESELECT PORT->Group[I2C_BB_SDA_PORT].DIR.reg |= (1 << I2C_BB_SDA_PIN)
#define SDA_INPUT (PORT->Group[I2C_BB_SDA_PORT].IN.reg & (1 << I2C_BB_SDA_PIN))

#define SCL_SETUP PORT->Group[I2C_BB_SCL_PORT].OUT.reg &= ~(1 << I2C_BB_SCL_PIN)
#define SCL_SELECT PORT->Group[I2C_BB_SCL_PORT].DIR.reg &= ~(1 << I2C_BB_SCL_PIN)
#define SCL_DESELECT PORT->Group[I2C_BB_SCL_PORT].DIR.reg |= (1 << I2C_BB_SCL_PIN)

static unsigned int baud = 0;

static void i2cDelay(void)
{
    unsigned long i, j;
    /*90 gives ~10uS at 48MHz*/

    for (i = 0; i < 90; i++)
        for (j = 0; j < 1; j++)
            asm("nop");
}

void i2cInit(unsigned int baudRate)
{
    baud = baudRate;

    SDA_SETUP;
    SDA_SELECT;

    SCL_SETUP;
    SCL_SELECT;

    /*Allow bus to stabalise after turning on radio power*/
    i2cDelay();
    i2cDelay();
    i2cDelay();
    i2cDelay();
    i2cDelay();
    i2cDelay();
    i2cDelay();
    i2cDelay();
    i2cDelay();
}

static void i2cStart(void)
{
    SDA_SELECT;
    SCL_SELECT;
    i2cDelay();
    SDA_DESELECT;
    i2cDelay();
    SCL_DESELECT;
    i2cDelay();
}

static void i2cStop(void)
{
    SDA_DESELECT;
    i2cDelay();
    SCL_SELECT;
    i2cDelay();
    SDA_SELECT;
    i2cDelay();
}

static void i2cWriteBit(unsigned char data)
{
    if (data > 0)
        SDA_SELECT;
    else
        SDA_DESELECT;

    i2cDelay();
    SCL_SELECT;
    i2cDelay();
    SCL_DESELECT;
}

static unsigned char i2cReadBit(void)
{
    unsigned char data;

    SDA_SELECT;
    i2cDelay();
    SCL_SELECT;
    i2cDelay();

    if (SDA_INPUT > 0)
        data = 1;
    else
        data = 0;

    SCL_DESELECT;
    i2cDelay();
    return data;
}

static unsigned char i2cWriteByte(unsigned char data, unsigned char start, unsigned char stop)
{
    unsigned char i;
    unsigned char ack;

    if (start)
        i2cStart();
    for (i = 0; i < 8; i++)
    {
        i2cWriteBit(data & 0x80);
        data <<= 1;
    }

    ack = i2cReadBit();

    i2cDelay();
    if (stop)
        i2cStop();

    return ack;
}

static unsigned char i2cReadByte(unsigned char ack, unsigned char stop)
{
    unsigned char i;
    unsigned char data = 0;

    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= i2cReadBit();
    }

    i2cDelay();
    if (ack)
        i2cWriteBit(0);
    else
        i2cWriteBit(1);

    if (stop)
        i2cStop();

    return data;
}

void i2cWrite(unsigned char addr, unsigned char subaddr, unsigned char value)
{
    i2cWriteByte(addr, 1, 1);
}

unsigned char i2cRead(unsigned char addr, unsigned char subaddr)
{
    if (i2cWriteByte(addr << 1, TRUE, FALSE)) /*Write command*/
    {
        if (i2cWriteByte(subaddr, FALSE, FALSE))
        {
            if (i2cWriteByte((addr << 1) | 0x01, TRUE, FALSE)) /*Read command*/
            {
                return i2cReadByte(FALSE, TRUE);
            }
        }
    }

    i2cStop();
    return 0;
}

unsigned char i2cRead16Addr(unsigned char addr, unsigned int subaddr)
{
    if (i2cWriteByte(addr << 1, TRUE, FALSE)) /*Write command*/
    {
        if (i2cWriteByte((subaddr >> 8) & 0xff, FALSE, FALSE))
        {
            if (i2cWriteByte(subaddr & 0xff, FALSE, FALSE))
            {
                if (i2cWriteByte((addr << 1) | 0x01, TRUE, FALSE)) /*Read command*/
                {
                    return i2cReadByte(FALSE, TRUE);
                }
            }
        }
    }

    i2cStop();
    return 0;
}
