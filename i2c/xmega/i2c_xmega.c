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

#include "system.h"
#include <i2c.h>
#include <debug.h>

#if UP_CLK == 32000000
static const unsigned int BAUD[] = {155, 35};
#endif
#if UP_CLK == 24000000
static const unsigned int BAUD[] = {116, 26};
#endif
#if UP_CLK == 8000000
static const unsigned int BAUD[] = {38, 8};
#endif
#if UP_CLK == 2000000
static const unsigned int BAUD[] = {14, 2};
#endif

static unsigned long timeoutCtr;

static unsigned long i2cReadTimeout = 10000;

//function initiates I2C1 module to baud rate BRG
void i2cInit(unsigned int baudRate)
{
  DEBUG_STR("i2cInit()...");
  TWIC_MASTER_CTRLA |= (1 << 3); /*Enable TWIC master*/
  TWIC_MASTER_BAUD = BAUD[baudRate];
  TWIC_MASTER_STATUS = TWI_MASTER_BUSSTATE_IDLE_gc; /*Force bus to IDLE state */

  DEBUG_STR("Done.\r\n");
}

void i2cWrite(unsigned char addr, unsigned char subaddr, unsigned char value)
{
  TWIC_MASTER_ADDR = addr; // write to RTC
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm))
    ;
  TWIC_MASTER_DATA = subaddr; // write word addr
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm))
    ;

  TWIC_MASTER_DATA = value;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm))
    ;

  TWIC_MASTER_CTRLC = TWI_MASTER_CMD_STOP_gc;
}

static unsigned char timeout(void)
{
  timeoutCtr++;
  if (timeoutCtr > i2cReadTimeout)
    return 1;
  else
    return 0;
}

void i2cWriteGeneric(unsigned char addr, unsigned char *data, unsigned char dataLen)
{
  unsigned char i;
  addr = addr & (~0x01);
  TWIC_MASTER_ADDR = addr; // write I2C address
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm))
    ;
  for (i = 0; i < dataLen; i++)
  {
    TWIC_MASTER_DATA = data[i]; // write word addr
    while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm))
      ;
  }

  TWIC_MASTER_CTRLC = TWI_MASTER_CMD_STOP_gc;
}

void i2cWrite16(unsigned char addr, unsigned int subaddr, unsigned int value)
{
  TWIC_MASTER_ADDR = addr & (~0x01); // write to RTC
  while ((TWIC_MASTER_STATUS & TWI_MASTER_RXACK_bm))
    ;

  TWIC_MASTER_DATA = (subaddr >> 8) & 0xff; // write word addr
  while ((TWIC_MASTER_STATUS & TWI_MASTER_RXACK_bm))
    ;

  TWIC_MASTER_DATA = subaddr & 0xff; // write word addr
  while ((TWIC_MASTER_STATUS & TWI_MASTER_RXACK_bm))
    ;

  TWIC_MASTER_DATA = value;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm))
    ;

  TWIC_MASTER_CTRLC = TWI_MASTER_CMD_STOP_gc;
}

unsigned char i2cRead(unsigned char addr, unsigned char subaddr)
{
  unsigned char temp = 0;
  uint8_t address = addr;

  TWIC_MASTER_ADDR = addr;
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm) || (TWIC.MASTER.STATUS & TWI_SLAVE_RXACK_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }

  TWIC_MASTER_DATA = subaddr; // write word addrpointer first
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm) || (TWIC.MASTER.STATUS & TWI_SLAVE_RXACK_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }

  TWIC_MASTER_ADDR = address | 0x01; // send read command
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_RIF_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }
  temp = TWIC_MASTER_DATA;

  TWIC_MASTER_CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;

  return temp;
}

unsigned int i2cRead16(unsigned char addr, unsigned int subaddr)
{
  unsigned int temp = 0;
  uint8_t address = addr;

  TWIC_MASTER_ADDR = addr;

  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm) || (TWIC.MASTER.STATUS & TWI_SLAVE_RXACK_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }
  TWIC_MASTER_DATA = (subaddr >> 8) & 0xff; // write word addrpointer first
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm) || (TWIC.MASTER.STATUS & TWI_SLAVE_RXACK_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }

  TWIC_MASTER_DATA = subaddr & 0xff; // write word addrpointer first
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm) || (TWIC.MASTER.STATUS & TWI_SLAVE_RXACK_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }

  //TWIC_MASTER_CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;

  TWIC_MASTER_ADDR = address | 0x01; // send read command
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_RIF_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }
  temp = (TWIC_MASTER_DATA << 8);
  TWIC_MASTER_CTRLC = TWI_MASTER_CMD_RECVTRANS_gc; /*Acknowledge byte*/
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_RIF_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }
  temp |= TWIC_MASTER_DATA;

  TWIC_MASTER_CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;

  return temp;
}

unsigned int i2cReadAd7997(unsigned char addr, unsigned char commandByte)
{
  unsigned int temp = 0;
  uint8_t address = addr;

  TWIC_MASTER_ADDR = addr;

  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm) || (TWIC.MASTER.STATUS & TWI_SLAVE_RXACK_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }

  TWIC_MASTER_DATA = commandByte; // write word addrpointer first
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm) || (TWIC.MASTER.STATUS & TWI_SLAVE_RXACK_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }

  TWIC_MASTER_ADDR = address | 0x01; // send read command
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_RIF_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }
  temp = TWIC_MASTER_DATA;
  temp = temp << 8;

  TWIC_MASTER_CTRLC = TWI_MASTER_CMD_RECVTRANS_gc; /*Acknowledge byte*/

  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_RIF_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }
  temp |= TWIC_MASTER_DATA;

  TWIC_MASTER_CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;

  return temp;
}

unsigned char i2cReadEeprom(unsigned char addr, unsigned int subaddr)
{
  unsigned char temp = 0;
  uint8_t address = addr;

  TWIC_MASTER_ADDR = addr;

  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm) || (TWIC.MASTER.STATUS & TWI_SLAVE_RXACK_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }
  TWIC_MASTER_DATA = (subaddr >> 8) & 0xff; // write word addrpointer first
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm) || (TWIC.MASTER.STATUS & TWI_SLAVE_RXACK_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }

  TWIC_MASTER_DATA = subaddr & 0xff; // write word addrpointer first
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm) || (TWIC.MASTER.STATUS & TWI_SLAVE_RXACK_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }

  //TWIC_MASTER_CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;

  TWIC_MASTER_ADDR = address | 0x01; // send read command
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_RIF_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }
  temp = TWIC_MASTER_DATA;

  TWIC_MASTER_CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;

  return temp;
}

unsigned int i2cNoAddrRead16(unsigned char addr)
{
  unsigned int temp = 0;
  uint8_t address = addr;

  TWIC_MASTER_ADDR = addr;
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm) || (TWIC.MASTER.STATUS & TWI_SLAVE_RXACK_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }
  //TWIC_MASTER_CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;

  TWIC_MASTER_ADDR = address | 0x01; // send read command
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_RIF_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }
  temp = (TWIC_MASTER_DATA << 8);

  TWIC_MASTER_CTRLC = TWI_MASTER_CMD_RECVTRANS_gc; /*Acknwledge byte*/

  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_RIF_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }
  temp |= TWIC_MASTER_DATA;

  TWIC_MASTER_CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;

  return temp;
}

/*Ping the specified address then hang up (issue stop)*/
void i2cPing(unsigned char addr)
{
  TWIC_MASTER_ADDR = addr; // write to RTC
  timeoutCtr = 0;
  while (!(TWIC_MASTER_STATUS & TWI_MASTER_WIF_bm))
  {
    if (timeout())
    {
      DEBUG_STR("i2cRead timeout\r\n");
      break;
    }
  }

  TWIC_MASTER_CTRLC = TWI_MASTER_CMD_STOP_gc;
}
