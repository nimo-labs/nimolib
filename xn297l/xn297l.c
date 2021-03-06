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
 * at86rf23xInit();
 * at86rf23xSetChannel(radioChan);
 * at86rf23xSetPanId(1);
 * at86rf23xSetAddr(1);
 *\endcode
 */

#include <nimolib.h>
#include <gpio.h>
#include <spi.h>
#include <delay.h>
#include <printf.h>

#include "xn297l.h"

static uint8_t xn297lReadReg(uint8_t reg);

void xn297lInit(void)
{
    printf("at86rf23xInit()...");

    /*Set CS pin as output and high*/
    GPIO_PIN_DIR(XN297L_SS_PORT, XN297L_SS_PIN, GPIO_DIR_OUT);
    GPIO_PIN_OUT(XN297L_SS_PORT, XN297L_SS_PIN, GPIO_OUT_HIGH);

    // /*Set SLPTR pin as output and low*/
    // GPIO_PIN_DIR(AT86RF23X_SLP_TR_PORT, AT86RF23X_SLP_TR_PIN, GPIO_DIR_OUT);
    // GPIO_PIN_OUT(AT86RF23X_SLP_TR_PORT, AT86RF23X_SLP_TR_PIN, GPIO_OUT_LOW);

    // /*Set RESET pin as output and low*/
    // GPIO_PIN_DIR(AT86RF23X_RESET_PORT, AT86RF23X_RESET_PIN, GPIO_DIR_OUT);
    // GPIO_PIN_OUT(AT86RF23X_RESET_PORT, AT86RF23X_RESET_PIN, GPIO_OUT_LOW);

    // delayMs(100);
    // GPIO_PIN_OUT(AT86RF23X_RESET_PORT, AT86RF23X_RESET_PIN, GPIO_OUT_HIGH);
    // delayMs(100);

    // at86rf23xWriteReg(AT86RF23X_REG_TRX_STATE, AT86RF23X_TRX_CMD_FORCE_TRX_OFF);

    // while (AT86RF23X_TRX_STATUS_TRX_OFF != (at86rf23xReadReg(AT86RF23X_REG_TRX_STATUS) & AT86RF23X_TRX_STATUS_TRX_STATUS_MASK))
    //     ;

    // //asm("WDR");

    // at86rf23xWriteReg(AT86RF23X_REG_TRX_CTRL_1, (1 << 5)); /*TX_AUTO_CRC_ON*/
    // //  AT86RF23XWriteReg(AT86RF23X_REG_PHY_TX_PWR, 0x00); /*+3dB*/

    // at86rf23xWriteReg(AT86RF23X_REG_RX_CTRL, 0x07); /*Set sensitivity to 3 when using Ant diversity, otherwise it should be 7*/
    // //    AT86RF23XWriteReg(AT86RF23XREG_ANT_DIV, (1 << 3) | (1 << 2)); /*Enable antenna diversity and ext switch*/

    // at86rf23xWriteReg(AT86RF23X_REG_IRQ_MASK, 0x00);
    // at86rf23xReadReg(AT86RF23X_REG_IRQ_STATUS);
    // at86rf23xWriteReg(AT86RF23X_REG_IRQ_MASK, AT86RF23X_TRX_END_MASK);
    // //at86rf23xWriteReg(AT86RF23XREG_IRQ_MASK, 0xff);

    // at86rf23xWriteReg(AT86RF23X_REG_TRX_CTRL_2, (1 << 7)); /*Set RX safe mode*/

    // printf("Done.\r\n");
    // printf("TRX Stat: 0x%.2X\r\n", at86rf23xReadReg(AT86RF23X_REG_TRX_STATUS));
    printf("Config reg: 0x%.2X\r\n", xn297lReadReg(XN297L_REG_EN_AA));
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