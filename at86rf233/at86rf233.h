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
* File: at86rf233.h
* Description: AT86RF233 / 2.4GHz transceiver driver
*/
#ifndef at86rf233H
#define at86rf233H

void at86rf233Init(void);

unsigned char at86rf233IsBusy(void);
void at86rf233SleepReq(void);
void at86rf233WakeupReq(void);

void at86rf233SetChannel(unsigned char channel);
void at86rf233SetPanId(unsigned int panId);
void at86rf233SetAddr(unsigned int shortAddr);
void at86rf233SetTxPower(unsigned int txPower);
void at86rf233TxData(unsigned char *data, unsigned char size);

unsigned char at86rf233ReadEd(void);

void at86rf233TransmitFrame(unsigned char data);

void at86rf233SetRxState(unsigned char state);

void at86rf233Process(void);

void at86rf233InterruptHandler(void);

unsigned char *at86rf233GetData(void);
unsigned char at86rf233DataAvailable(void);

void at86rf233SetClkmFreq(unsigned char frequency);

void at86rf233EncryptData(unsigned char *data, unsigned char *key);
void at86rf233DecryptData(unsigned char *data, unsigned char *key);

#define AT86RF233MAX_PAYLOAD_SIZE 128
#define AT86RF233AES_BLOCK_SIZE 16

#define AT86RF233CMD_READ_REG 0x80
#define AT86RF233CMD_WRITE_REG 0xC0
#define AT86RF233CMD_READ_FRAME 0x20
#define AT86RF233CMD_WRITE_FRAME 0x60
#define AT86RF233CMD_READ_SRAM 0x00
#define AT86RF233CMD_WRITE_SRAM 0x40

#define AT86RF233TRX_CMD_NOP 0
#define AT86RF233TRX_CMD_TX_START 2
#define AT86RF233TRX_CMD_FORCE_TRX_OFF 3
#define AT86RF233TRX_CMD_FORCE_PLL_ON 4
#define AT86RF233TRX_CMD_RX_ON 6
#define AT86RF233TRX_CMD_TRX_OFF 8
#define AT86RF233TRX_CMD_PLL_ON 9
#define AT86RF233TRX_CMD_RX_AACK_ON 22
#define AT86RF233TRX_CMD_TX_ARET_ON 25

#define AT86RF233CLKM_OFF 0
#define AT86RF233CLKM_1MHZ 1
#define AT86RF233CLKM_2MHZ 2
#define AT86RF233CLKM_4MHZ 3
#define AT86RF233CLKM_8MHZ 4
#define AT86RF233CLKM_16MHZ 5
#define AT86RF233CLKM_250KHZ 6
#define AT86RF233CLKM_62KHZ 7

#define AT86RF233TRX_STATUS_P_ON 0
#define AT86RF233TRX_STATUS_BUSY_RX 1
#define AT86RF233TRX_STATUS_BUSY_TX 2
#define AT86RF233TRX_STATUS_RX_ON 6
#define AT86RF233TRX_STATUS_TRX_OFF 8
#define AT86RF233TRX_STATUS_PLL_ON 9
#define AT86RF233TRX_STATUS_SLEEP 15
#define AT86RF233TRX_STATUS_BUSY_RX_AACK 17
#define AT86RF233TRX_STATUS_BUSY_TX_ARET 18
#define AT86RF233TRX_STATUS_RX_AACK_ON 22
#define AT86RF233TRX_STATUS_TX_ARET_ON 25
#define AT86RF233TRX_STATUS_RX_ON_NOCLK 28
#define AT86RF233TRX_STATUS_RX_AACK_ON_NOCLK 29
#define AT86RF233TRX_STATUS_BUSY_RX_AACK_NOCLK 30
#define AT86RF233TRX_STATUS_STATE_TRANSITION_IN_PROGRESS 31
#define AT86RF233TRX_STATUS_TRX_STATUS_MASK 0x1f

#define AT86RF233PLL_LOCK_MASK (1 << 0)
#define AT86RF233PLL_UNLOCK_MASK (1 << 1)
#define AT86RF233RX_START_MASK (1 << 2)
#define AT86RF233TRX_END_MASK (1 << 3)
#define AT86RF233TRX_UR_MASK (1 << 6)
#define AT86RF233BAT_LOW_MASK (1 << 7)

#define AT86RF233REG_TRX_STATUS 0x01
#define AT86RF233REG_TRX_STATE 0x02
#define AT86RF233REG_TRX_CTRL_0 0x03
#define AT86RF233REG_TRX_CTRL_1 0x04
#define AT86RF233REG_PHY_TX_PWR 0x05
#define AT86RF233REG_PHY_RSSI 0x06
#define AT86RF233REG_PHY_ED_LEVEL 0x07
#define AT86RF233REG_PHY_CC_CCA 0x08
#define AT86RF233REG_CCA_THRESH 0x09
#define AT86RF233REG_RX_CTRL 0x0A
#define AT86RF233REG_TRX_CTRL_2 0x0C
#define AT86RF233REG_ANT_DIV 0x0D
#define AT86RF233REG_IRQ_MASK 0x0E
#define AT86RF233REG_IRQ_STATUS 0x0F
#define AT86RF233REG_VREG_CTRL 0x10
#define AT86RF233REG_BATMON 0x11
#define AT86RF233REG_XOSC_CTRL 0x12
#define AT86RF233REG_XAH_CTRL_1 0x17
#define AT86RF233REG_PLL_CF 0x1a
#define AT86RF233REG_PLL_DCU 0x1b
#define AT86RF233REG_PART_NUM 0x1c
#define AT86RF233REG_VERSION_NUM 0x1d
#define AT86RF233REG_MAN_ID_0 0x1e
#define AT86RF233REG_MAN_ID_1 0x1f
#define AT86RF233REG_SHORT_ADDR_0 0x20
#define AT86RF233REG_SHORT_ADDR_1 0x21
#define AT86RF233REG_PAN_ID_0 0x22
#define AT86RF233REG_PAN_ID_1 0x23
#define AT86RF233REG_IEEE_ADDR_0 0x24
#define AT86RF233REG_IEEE_ADDR_1 0x25
#define AT86RF233REG_IEEE_ADDR_2 0x26
#define AT86RF233REG_IEEE_ADDR_3 0x27
#define AT86RF233REG_IEEE_ADDR_4 0x28
#define AT86RF233REG_IEEE_ADDR_5 0x29
#define AT86RF233REG_IEEE_ADDR_6 0x2a
#define AT86RF233REG_IEEE_ADDR_7 0x2b
#define AT86RF233REG_CSMA_SEED_0 0x2d
#define AT86RF233REG_CSMA_SEED_1 0x2e

#define AT86RF233IRQ_STATE_BAT_LOW 0x80
#define AT86RF233IRQ_STATE_TRX_UR 0x40
#define AT86RF233IRQ_STATE_TRX_END 0x08
#define AT86RF233IRQ_STATE_RX_START 0x04
#define AT86RF233IRQ_STATE_PLL_UNLOCK 0x02
#define AT86RF233IRQ_STATE_PLL_LOCK 0x01

#define AT86RF233REG_AES_STATUS 0x82
#define AT86RF233REG_AES_CTRL 0x83
#define AT86RF233REG_AES_DATA_START 0x84
#define AT86RF233REG_AES_DATA_ADDR_END 0x93
#define AT86RF233REG_AES_CTRL_MIRROR 0x94

#define AT86RF233AES_CTRL_REQUEST 0x07
#define AT86RF233AES_CTRL_MODE 0x04
#define AT86RF233AES_CTRL_DIR 0x03

#define AT86RF233AES_CTRL_MODE_ECB 0x00
#define AT86RF233AES_CTRL_MODE_KEY 0x01
#define AT86RF233AES_CTRL_MODE_CBC 0x02
#endif