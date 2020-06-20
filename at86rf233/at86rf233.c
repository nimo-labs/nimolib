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
* File: at86rf233.c
* Description: AT86RF233 / 2.4GHz transceiver driver
*/
/**
 * The driver requires the following I/O defines to be present in the projects system.h
 * \code
 * #define  BB_SPI_CHAN0
 *
 * #define AT86RF233_CS_PORT SAM_GPIO_PORTB
 * #define AT86RF233_CS_PIN 2
 *
 *  #define AT86RF233_SLP_TR_PORT SAM_GPIO_PORTB
 * #define AT86RF233_SLP_TR_PIN 1
 *
 * #define AT86RF233_RESET_PORT SAM_GPIO_PORTB
 * #define AT86RF233_RESET_PIN 0
 * \endcode
 *
 *Basic setup of the chip involves the following:
 *\code
 *AT86RF233Init();
 *AT86RF233SetChannel(radioChan);
 *AT86RF233SetPanId(1);
 *AT86RF233SetAddr(1);
 *\endcode
 */

#include <sam.h>
#include <nimolib.h>
#include <gpio.h>
#include <spi.h>
#include <delay.h>
#include <printf.h>

#include "at86rf233.h"

#define FRAME_BUF_LEN 4
#define FRAME_LEN 128

static volatile unsigned char frameBufStart = 0;
static volatile unsigned char frameBufEnd = 0;

static unsigned char dataFrameBuf[FRAME_BUF_LEN][FRAME_LEN];

static unsigned char at86rf233ReadReg(unsigned char regAddr);
static void at86rf233WriteReg(unsigned char regAddr, unsigned char data);

static void at86rf233SetTrxState(unsigned char state);
static unsigned char at86rf233ReadReg(unsigned char regAddr);

static unsigned char at86rf233GenRndByte(void);

static volatile unsigned char txBusy = 0; /*if 1 then transmit is busy*/
static volatile unsigned char rxData = 0; /*if 1 there there is data ready*/

static volatile unsigned char pktValid = 0; /*If 1 then the last packet was valid */

unsigned char encKey[AT86RF233AES_BLOCK_SIZE];
unsigned char decKey[AT86RF233AES_BLOCK_SIZE];
unsigned char nonce[AT86RF233AES_BLOCK_SIZE] = "123456789ABCDEF";

static void xorStr(unsigned char *text, unsigned char *nonce)
{
    unsigned char i;
    for (i = 0; i < AT86RF233AES_BLOCK_SIZE; i++)
        text[i] = text[i] ^ nonce[i];
}

static void at86rf233PollAesStatus(void)
{
    unsigned char result = 0;
    while (!result)
    {
        GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_LOW);
        spiTxByte(AT86RF233CMD_READ_SRAM);
        spiTxByte(AT86RF233REG_AES_STATUS);
        spiTxByte(((AT86RF233AES_CTRL_MODE_KEY << 4) | (0 << 3)));
        result = (spiRxByte() & 0x01);
        GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);
    }
}

void at86rf233EncryptData(unsigned char *data, unsigned char *key)
{
    unsigned char i;

    printf("Key:\r\n");
    debugHexArray(AT86RF233AES_BLOCK_SIZE, key);
    printf("In:\r\n");
    debugHexArray(AT86RF233AES_BLOCK_SIZE, data);

    /*Store the encryption key*/
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(AT86RF233CMD_WRITE_SRAM);
    spiTxByte(AT86RF233REG_AES_CTRL);
    spiTxByte(
        ((AT86RF233AES_CTRL_MODE_KEY << AT86RF233AES_CTRL_MODE) | (0 << AT86RF233AES_CTRL_DIR)));
    for (i = 0; i < AT86RF233AES_BLOCK_SIZE; i++)
        spiTxByte(key[i]);
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);

    //  xorStr(data, nonce);

    /*Encrypt data with our key */
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(AT86RF233CMD_WRITE_SRAM);
    spiTxByte(AT86RF233REG_AES_CTRL);
    spiTxByte(
        (AT86RF233AES_CTRL_MODE_CBC << AT86RF233AES_CTRL_MODE) | (0 << AT86RF233AES_CTRL_DIR)); /*Set ECB mode and direction to encrypt*/
    for (i = 0; i < AT86RF233AES_BLOCK_SIZE; i++)
        spiTxByte(data[i]);
    spiTxByte(
        (1 << AT86RF233AES_CTRL_REQUEST) | (AT86RF233AES_CTRL_MODE_CBC << AT86RF233AES_CTRL_MODE) | (0 << AT86RF233AES_CTRL_DIR)); /*Start encryption*/
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);
    /*Wait for encryption to finish */
    //   AT86RF233PollAesStatus();
    delayMs(500);

    for (i = 0; i < 16; i++)
        data[i] = 0x00;

    /*Read out data*/
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(AT86RF233CMD_READ_SRAM);
    spiTxByte(AT86RF233REG_AES_DATA_START);
    for (i = 0; i < AT86RF233AES_BLOCK_SIZE; i++)
        data[i] = spiRxByte();
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);

    printf("Out:\r\n");
    debugHexArray(AT86RF233AES_BLOCK_SIZE, &data[0]);
}

void at86rf233DecryptData(unsigned char *data, unsigned char *key)
{
    unsigned char i;
    /*Store the encryption key*/
    printf("IN:\r\n");
    debugHexArray(AT86RF233AES_BLOCK_SIZE, data);
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(AT86RF233CMD_WRITE_SRAM);
    spiTxByte(AT86RF233REG_AES_CTRL);
    spiTxByte(
        ((AT86RF233AES_CTRL_MODE_KEY << AT86RF233AES_CTRL_MODE) | (0 << AT86RF233AES_CTRL_DIR)));
    for (i = 0; i < 16; i++)
        spiTxByte(key[i]);
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);

    /*Decrypt data with our key */
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(AT86RF233CMD_WRITE_SRAM);
    spiTxByte(AT86RF233REG_AES_CTRL);
    spiTxByte(
        (AT86RF233AES_CTRL_MODE_CBC << AT86RF233AES_CTRL_MODE) | (0 << AT86RF233AES_CTRL_DIR)); /*Set ECB mode and direction to encrypt*/
    for (i = 0; i < 16; i++)
        spiTxByte(data[i]);
    spiTxByte(
        (1 << AT86RF233AES_CTRL_REQUEST) | (AT86RF233AES_CTRL_MODE_CBC << AT86RF233AES_CTRL_MODE) | (0 << AT86RF233AES_CTRL_DIR)); /*Start decryption*/
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);
    /*Wait for decryption to finish */
    AT86RF233PollAesStatus();
    //   delay(65000);
    for (i = 0; i < 16; i++)
        data[i] = 0x00;

    /*Read out data*/
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(AT86RF233CMD_READ_SRAM);
    spiTxByte(AT86RF233REG_AES_DATA_START);
    for (i = 0; i < 16; i++)
        data[i] = spiRxByte();
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);

    // xorStr(data, nonce);

    printf("Out:\r\n");
    debugHexArray(AT86RF233AES_BLOCK_SIZE, data);
}

unsigned char at86rf233IsBusy(void)
{
    return txBusy;
}

void at86rf233SleepReq(void)
{
    AT86RF233SetTrxState(AT86RF233TRX_CMD_TRX_OFF);
    GPIO_PIN_OUT(AT86RF233_SLP_TR_PORT, AT86RF233_SLP_TR_PIN, GPIO_OUT_HIGH);
}

void at86rf233WakeupReq(void)
{
    GPIO_PIN_OUT(AT86RF233_SLP_TR_PORT, AT86RF233_SLP_TR_PIN, GPIO_OUT_LOW);

    AT86RF233SetTrxState(AT86RF233TRX_CMD_TRX_OFF);
    AT86RF233ReadReg(AT86RF233REG_IRQ_STATUS);

    AT86RF233SetTrxState(AT86RF233TRX_CMD_RX_AACK_ON);
}

void at86rf233Init(void)
{
    printf("at86rf233Init()...");

    /*Set CS pin as output and high*/
    GPIO_PIN_DIR(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_DIR_OUT);
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);

    /*Set SLPTR pin as output and low*/
    GPIO_PIN_DIR(AT86RF233_SLP_TR_PORT, AT86RF233_SLP_TR_PIN, GPIO_DIR_OUT);
    GPIO_PIN_OUT(AT86RF233_SLP_TR_PORT, AT86RF233_SLP_TR_PIN, GPIO_OUT_LOW);

    /*Set RESET pin as output and low*/
    GPIO_PIN_DIR(AT86RF233_RESET_PORT, AT86RF233_RESET_PIN, GPIO_DIR_OUT);
    GPIO_PIN_OUT(AT86RF233_RESET_PORT, AT86RF233_RESET_PIN, GPIO_OUT_LOW);

    delayMs(100);
    GPIO_PIN_OUT(AT86RF233_RESET_PORT, AT86RF233_RESET_PIN, GPIO_OUT_LOW);
    delayMs(100);

    AT86RF233WriteReg(AT86RF233REG_TRX_STATE, AT86RF233TRX_CMD_FORCE_TRX_OFF);

    while (AT86RF233TRX_STATUS_TRX_OFF != (AT86RF233ReadReg(AT86RF233REG_TRX_STATUS) & AT86RF233TRX_STATUS_TRX_STATUS_MASK))
        ;

    //asm("WDR");

    AT86RF233WriteReg(AT86RF233REG_TRX_CTRL_1, (1 << 5)); /*TX_AUTO_CRC_ON*/
    //  AT86RF233WriteReg(AT86RF233_REG_PHY_TX_PWR, 0x00); /*+3dB*/

    AT86RF233WriteReg(AT86RF233REG_RX_CTRL, 0x07); /*Set sensitivity to 3 when using Ant diversity, otherwise it should be 7*/
    //    AT86RF233WriteReg(AT86RF233REG_ANT_DIV, (1 << 3) | (1 << 2)); /*Enable antenna diversity and ext switch*/

    AT86RF233WriteReg(AT86RF233REG_IRQ_MASK, 0x00);
    AT86RF233ReadReg(AT86RF233REG_IRQ_STATUS);
    AT86RF233WriteReg(AT86RF233REG_IRQ_MASK, AT86RF233TRX_END_MASK);
    //AT86RF233WriteReg(AT86RF233REG_IRQ_MASK, 0xff);

    AT86RF233WriteReg(AT86RF233REG_TRX_CTRL_2, (1 << 7)); /*Set RX safe mode*/

    printf("Done.\r\n");
    printf("TRX Stat: 0x%.sX\r\n", AT86RF233ReadReg(AT86RF233REG_TRX_STATUS));
    printf("Part num: 0x%.sX\r\n", AT86RF233ReadReg(AT86RF233REG_PART_NUM));
}

void at86rf233SetClkmFreq(unsigned char frequency)
{
    unsigned char tmpTrxCtrl0;

    tmpTrxCtrl0 = AT86RF233ReadReg(AT86RF233REG_TRX_CTRL_0);
    tmpTrxCtrl0 &= 0xf0;      /*Clear CLKM_CTRL and CLKM_SHA_SEL to ensure immediate clock switch*/
    tmpTrxCtrl0 |= frequency; /*Set new CLKM_CTRL value*/
    at86rf233WriteReg(AT86RF233REG_TRX_CTRL_0, tmpTrxCtrl0);
}

void at86rf233StoreData(void)
{
    unsigned char dLen;
    unsigned char i;

    if (rxData)
    {
        rxData = 0;
        GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_LOW);
        spiTxByte(AT86RF233CMD_READ_FRAME);
        dLen = spiRxByte();
        dLen -= 2; /*Remove CRC from end of data*/

        dataFrameBuf[frameBufStart][0] = dLen;
        i = 1;

        while (dLen > 0)
        {
            dataFrameBuf[frameBufStart][i] = spiRxByte();
            i++;
            dLen--;
        }
        if (frameBufStart < FRAME_BUF_LEN - 1)
            frameBufStart++;
        else
            frameBufStart = 0;
        GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);
    }
}

unsigned char *at86rf233GetData(void)
{
    unsigned char *tmpPtr = &dataFrameBuf[frameBufEnd][0];
    if (frameBufEnd < FRAME_BUF_LEN - 1)
        frameBufEnd++;
    else
        frameBufEnd = 0;

    return tmpPtr;
}

void at86rf233TransmitFrame(unsigned char data)
{
    if (!txBusy)
    {
        AT86RF233WriteReg(AT86RF233REG_TRX_STATE, AT86RF233TRX_CMD_PLL_ON);
        while (AT86RF233TRX_STATUS_PLL_ON != (AT86RF233ReadReg(AT86RF233REG_TRX_STATUS) & AT86RF233TRX_STATUS_TRX_STATUS_MASK))
            ;
        printf("PLL_ON\r\n");

        GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_LOW);
        spiTxByte(AT86RF233CMD_WRITE_FRAME);

        spiTxByte('\003');
        spiTxByte(data);
        GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);

        txBusy = 1;
        AT86RF233WriteReg(AT86RF233REG_TRX_STATE, AT86RF233TRX_CMD_TX_START);

        printf("TX Frame done.\r\n");
    }
}

unsigned char at86rf233DataAvailable(void)
{
    if (frameBufStart != frameBufEnd)
        return 1;
    else
        return 0;
}

static void at86rf233SetTrxState(unsigned char state)
{
    at86rf233WriteReg(AT86RF233REG_TRX_STATE, AT86RF233TRX_CMD_FORCE_TRX_OFF);
    while (AT86RF233TRX_STATUS_TRX_OFF != (at86rf233ReadReg(AT86RF233REG_TRX_STATUS) & AT86RF233TRX_STATUS_TRX_STATUS_MASK))
        ;
    at86rf233WriteReg(AT86RF233REG_TRX_STATE, state);
    while (state != (at86rf233ReadReg(AT86RF233REG_TRX_STATUS) & AT86RF233TRX_STATUS_TRX_STATUS_MASK))
        ;
}

void at86rf233SetRxState(unsigned char state)
{
    at86rf233SetTrxState(AT86RF233TRX_CMD_TRX_OFF);

    if (state)
        at86rf233SetTrxState(AT86RF233TRX_CMD_RX_ON);
    else
        at86rf233SetTrxState(AT86RF233TRX_CMD_TRX_OFF);
}

void at86rf233SetChannel(unsigned char channel)
{
    //    channel += 0x0B; /*Shift the channel into the allowed range for the radio chip*/
    printf("at86rf233SetChannel(0x%.2X)...", channel);
    at86rf233SetTrxState(AT86RF233TRX_CMD_TRX_OFF);
    unsigned char v = at86rf233ReadReg(AT86RF233REG_PHY_CC_CCA) & ~0x1f;
    at86rf233WriteReg(AT86RF233REG_PHY_CC_CCA, v | channel);
    at86rf233SetRxState(1);
    printf("Done.\r\n");
}

void at86rf233SetPanId(unsigned int panId)
{
    printf("at86rf233SetPanId...");
    at86rf233SetTrxState(AT86RF233TRX_CMD_TRX_OFF);
    at86rf233WriteReg(AT86RF233REG_PAN_ID_0, panId & 0xff);
    at86rf233WriteReg(AT86RF233REG_PAN_ID_1, (panId >> 8) & 0xff);
    at86rf233SetRxState(1);
    printf("Done.\r\n");
}

void at86rf233SetAddr(unsigned int shortAddr)
{
    printf("at86rf233SetAddr...");
    at86rf233SetTrxState(AT86RF233TRX_CMD_TRX_OFF);
    at86rf233WriteReg(AT86RF233REG_SHORT_ADDR_0, shortAddr & 0xff);
    at86rf233WriteReg(AT86RF233REG_SHORT_ADDR_1, (shortAddr >> 8) & 0xff);
    at86rf233WriteReg(AT86RF233REG_CSMA_SEED_0,
                      (shortAddr >> 8) + (shortAddr & 0xff));
    at86rf233SetRxState(1);
    printf("Done.\r\n");
}

void at86rf233SetTxPower(unsigned int txPower)
{
    at86rf233SetTrxState(AT86RF233TRX_CMD_TRX_OFF);
    unsigned char v = at86rf233ReadReg(AT86RF233REG_PHY_TX_PWR) & ~0x0f;
    at86rf233WriteReg(AT86RF233REG_PHY_TX_PWR, v | txPower);
    at86rf233SetRxState(1);
}

void at86rf233TxData(unsigned char *data, unsigned char size)
{
    unsigned char i;

    if (size < AT86RF233MAX_PAYLOAD_SIZE)
    {
        if (!txBusy)
        {
            txBusy = 1;
            at86rf233SetTrxState(AT86RF233TRX_CMD_TX_ARET_ON);
            GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_LOW);

            spiTxByte(AT86RF233CMD_WRITE_FRAME);
            spiTxByte(size + 2); /*+2 to account for the CRC*/
            for (i = 0; i < size; i++)
                spiTxByte(data[i]);

            GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);

            at86rf233WriteReg(AT86RF233REG_TRX_STATE,
                              AT86RF233TRX_CMD_TX_START);
        }
    }
}

unsigned char at86rf233ReadEd(void)
{
    return at86rf233ReadReg(AT86RF233REG_PHY_ED_LEVEL);
}

static unsigned char at86rf233ReadReg(unsigned char regAddr)
{
    unsigned char data;
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_LOW);
    //    printf("1\r\n");
    spiTxByte(AT86RF233CMD_READ_REG | regAddr);
    //    printf("2\r\n");
    data = spiRxByte();
    //    printf("3\r\n");
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);
    //    printf("4\r\n");

    return data;
}

static void at86rf233WriteReg(unsigned char regAddr, unsigned char data)
{
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_LOW);
    spiTxByte(AT86RF233CMD_WRITE_REG | regAddr);
    spiTxByte(data);
    GPIO_PIN_OUT(AT86RF233_CS_PORT, AT86RF233_CS_PIN, GPIO_OUT_HIGH);
}

void at86rf233InterruptHandler(void)
{
    unsigned char atIrqState;

    //   delayMs(1); /*delay required here to prevent duplicate rx buffer reads. Conflict between sam and at86 int systems??*/

    atIrqState = at86rf233ReadReg(AT86RF233REG_IRQ_STATUS);

    //   atIrqState &= AT86RF233IRQ_STATE_TRX_END; /*We are only interested in the TRX state*/
    switch (atIrqState)
    {
    case AT86RF233IRQ_STATE_BAT_LOW:
        break;
    case AT86RF233IRQ_STATE_TRX_UR:
        break;
    case AT86RF233IRQ_STATE_TRX_END:
        //     printf("at86int: RX TRX_END\r\n");
        if (txBusy)
        {
            txBusy = 0;
            at86rf233SetRxState(1);
        }
        else
        {
            if (0 == rxData)
            {
                rxData = 1;
                at86rf233StoreData();
            }
        }
        break;
    case AT86RF233IRQ_STATE_RX_START:
        //        printf("at86int: RX_START\r\n");
        break;
    case AT86RF233IRQ_STATE_PLL_UNLOCK:
        break;
    case AT86RF233IRQ_STATE_PLL_LOCK:
        break;
    default:
        break;
    }
}

static unsigned char at86rf233GenRndByte(void)
{
    unsigned char byte = 0;
    unsigned char i;
    unsigned char intState = 0;

    intState = at86rf233ReadReg(AT86RF233REG_IRQ_MASK); /*Read old value of int mask*/
    at86rf233WriteReg(AT86RF233REG_IRQ_MASK, 0x00);     /*Disable interrupts*/

    at86rf233WriteReg(AT86RF233REG_TRX_STATE, AT86RF233TRX_CMD_RX_ON);
    while (AT86RF233TRX_STATUS_RX_ON != (AT86RF233ReadReg(AT86RF233REG_TRX_STATUS) & AT86RF233TRX_STATUS_TRX_STATUS_MASK))
        ;

    for (i = 0; i < 4; i++)
    {
        delay(1000); /*Must be min 1uS*/
        byte |= ((at86rf233ReadReg(AT86RF233REG_PHY_RSSI) >> 5) & 0x03)
                << (i * 2);
    }

    at86rf233WriteReg(AT86RF233REG_IRQ_MASK, intState); /*Reset interrupts*/

    return byte;
}
