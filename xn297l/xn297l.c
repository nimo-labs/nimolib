/*
* Copyright 2021 NimoLabs Ltd.
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
* File: xn297l.c
* Description: XN297L / 2.4GHz transceiver driver
*/
/**
 * The driver requires the following I/O defines to be present in the projects nimolib.h
 * \code
 * #define  BB_SPI_CHAN0
 *
 * XN297L_SPI_CHAN
 * XN297L_SS_PORT
 * XN297L_SS_PIN
 *
 * \endcode
 *
 *Basic setup of the chip involves the following:
 *\code

 *\endcode
 */

#include <nimolib.h>
#include <gpio.h>
#include <spi.h>
#include <delay.h>
#include <printf.h>

#include "xn297l.h"

static uint8_t xn297lReadReg(uint8_t reg);
static uint8_t xn297lWriteReg(uint8_t reg, uint8_t value);

void xn297lInit(void)
{
    printf("xn297lInit()...");

    /*Set CS pin as output and high*/
    GPIO_PIN_DIR(XN297L_SS_PORT, XN297L_SS_PIN, GPIO_DIR_OUT);
    GPIO_PIN_OUT(XN297L_SS_PORT, XN297L_SS_PIN, GPIO_OUT_HIGH);

    /*Force normal burst mode*/
    xn297lWriteReg(XN297L_REG_EN_AA, 0x00);
    xn297lWriteReg(XN297L_REG_SETUP_RETR, 0x00);
    xn297lWriteReg(XN297L_REG_DYNPD, 0x00);
    uint8_t featureReg=xn297lReadReg(XN297L_REG_FEATURE);
    xn297lWriteReg(XN297L_REG_FEATURE, featureReg & 0x07);


    /*Set address len to 3 bytes */
    // xn297lWriteReg(XN297L_REG_SETUP_AW, 0x01);

    /*Set recieve payload to 64 bytes*/
    xn297lWriteReg(XN297L_REG_RX_PW_P0, 64);

    /*Power up Radio*/
    uint8_t configReg=0x00;
    configReg |= (1 << 1 /* PWR_UP*/);
    xn297lWriteReg(XN297L_REG_CONFIG, configReg);

    printf("Config reg: 0x%.2X\r\n", xn297lReadReg(XN297L_REG_CONFIG));
    printf("Status reg: 0x%.2X\r\n", xn297lReadReg(XN297L_REG_STATUS));

    printf("Done.\r\n");
}

void xn297lEnRxMode(void)
{
    uint8_t configReg=xn297lReadReg(XN297L_REG_CONFIG);

    configReg |= (1 << 7 /*EN_PM*/) | (1 << 1 /*PWR_UP*/) | (1 << 0 /* PRIM_RX*/);
    xn297lWriteReg(XN297L_REG_CONFIG, configReg);
}

void xn297lReadStatus(void)
{
    printf("Status reg: 0x%.2X\r\n", xn297lReadReg(XN297L_REG_STATUS));
}

void xn297lSetChannel(uint8_t rfChannel)
{
    uint8_t rfSetupReg=xn297lReadReg(XN297L_REG_RFSETUP);

    rfSetupReg &= ~(0x3F << 0); /*Clear channel*/
    rfSetupReg |= (rfChannel << 0); /*Set channel*/
    xn297lWriteReg(XN297L_REG_RFSETUP, rfSetupReg);
}

void xn297lSetDataRate(uint8_t dataRate)
{
    uint8_t rfSetupReg=xn297lReadReg(XN297L_REG_RFSETUP);

    rfSetupReg &= ~(0x03 << 6); /*Clear datarate*/
    rfSetupReg |= (dataRate << 6); /*Set datarate*/
    xn297lWriteReg(XN297L_REG_RFSETUP, rfSetupReg);
}

void xn297lSetTxPower(uint8_t power)
{
    uint8_t rfSetupReg=xn297lReadReg(XN297L_REG_RFSETUP);

    rfSetupReg &= ~(0x3F << 0); /*Clear Tx power*/
    rfSetupReg |= (0x2A << 0); /*Set TX Power*/
    xn297lWriteReg(XN297L_REG_RFSETUP, rfSetupReg);
}


uint8_t xn297lWriteReg(uint8_t reg, uint8_t value)
{
    uint8_t regData=0;

    GPIO_PIN_OUT(XN297L_SS_PORT, XN297L_SS_PIN, GPIO_OUT_LOW);
    spiTxByte(XN297L_SPI_CHAN, XN297L_CMD_WRITE_REG | reg);
    spiTxByte(XN297L_SPI_CHAN, value);
    GPIO_PIN_OUT(XN297L_SS_PORT, XN297L_SS_PIN, GPIO_OUT_HIGH);

    return regData;
}

uint8_t xn297lReadReg(uint8_t reg)
{
    uint8_t regData=0;

    GPIO_PIN_OUT(XN297L_SS_PORT, XN297L_SS_PIN, GPIO_OUT_LOW);
    spiTxByte(XN297L_SPI_CHAN, XN297L_CMD_READ_REG | reg);
    regData = spiRxByte(XN297L_SPI_CHAN);
    GPIO_PIN_OUT(XN297L_SS_PORT, XN297L_SS_PIN, GPIO_OUT_HIGH);

    return regData;
}

uint8_t xn297lTransmitData(uint8_t *data, uint8_t len)
{
    if(!(xn297lReadReg(XN297L_REG_STATUS) & (1 << 0))) /*Check for TX FIFO Full*/
    {
        /*Enable TX Mode*/
        uint8_t configReg=xn297lReadReg(XN297L_REG_CONFIG);
        configReg &= ~(1 << 0 /* PRIM_RX*/);
        xn297lWriteReg(XN297L_REG_CONFIG, configReg);

        GPIO_PIN_OUT(XN297L_SS_PORT, XN297L_SS_PIN, GPIO_OUT_LOW);
        spiTxByte(XN297L_SPI_CHAN, XN297L_CMD_TX_PAYLOAD);

        for(uint8_t i=0; i < len; i++)
            spiTxByte(XN297L_SPI_CHAN, data[i]);
        GPIO_PIN_OUT(XN297L_SS_PORT, XN297L_SS_PIN, GPIO_OUT_HIGH);

        delayMs(25);

        uint8_t statusReg=xn297lReadReg(XN297L_REG_STATUS);
        if(statusReg & (1 << 5))
        {
            // TX_DATA_SENT
            // printf("data sent\r\n");
            // clear TX_DATA_SENT (write 1 to clear)
            statusReg |= (1 << 5);
            xn297lWriteReg(XN297L_REG_STATUS, statusReg);

        }
        else
        {
            printf("data not sent\r\n");;
        }

        /*Enable RX mode*/
        configReg=xn297lReadReg(XN297L_REG_CONFIG);
        configReg |= (1 << 0 /* PRIM_RX*/);
        xn297lWriteReg(XN297L_REG_CONFIG, configReg);
    }
    else
    {
        printf("Transmit: FIFO full\r\n");
        return -1;
    }
    return 0;
}

int8_t xn297lReceiveData(uint8_t *data, uint8_t bufMaxLen)
{
    if(!(xn297lReadReg(XN297L_REG_STATUS) & 0x40)) /*Check for TX FIFO Full*/
        return 0; /*No data avilable */
    else
    {
        uint8_t dataLen=xn297lReadReg(XN297L_REG_RX_PW_P0);
        if(dataLen > bufMaxLen)
            return -1;
        GPIO_PIN_OUT(XN297L_SS_PORT, XN297L_SS_PIN, GPIO_OUT_LOW);
        spiTxByte(XN297L_SPI_CHAN, XN297L_CMD_RX_PAYLOAD);

        for(int i=0; i<dataLen; i++)
        {
            data[i] = spiRxByte(XN297L_SPI_CHAN);
        }

        GPIO_PIN_OUT(XN297L_SS_PORT, XN297L_SS_PIN, GPIO_OUT_HIGH);

        uint8_t statusReg=xn297lReadReg(XN297L_REG_STATUS);
        // clear TX_DATA_SENT (write 1 to clear)
        statusReg |= (1 << 6);
        xn297lWriteReg(XN297L_REG_STATUS, statusReg);
        return dataLen;
    }
}

void xn297lPwrDown(void)
{
    uint8_t configReg = xn297lReadReg(XN297L_REG_CONFIG);
    configReg &= ~(1 << 7); /*Clear EN_PM bit*/
    configReg &= ~(1 << 1); /*Clear PWR_UP bit*/
    xn297lWriteReg(XN297L_REG_CONFIG, configReg);
}