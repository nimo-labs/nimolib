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
* File: at86rf23x.h
* Description: AT86RF23X / 2.4GHz transceiver driver
*/
#ifndef at86rf23xH
#define at86rf23xH

void at86rf23xInit(void);

unsigned char at86rf23xIsBusy(void);
void at86rf23xSleepReq(void);
void at86rf23xWakeupReq(void);

void at86rf23xSetChannel(unsigned char channel);
void at86rf23xSetPanId(unsigned int panId);
void at86rf23xSetAddr(unsigned int shortAddr);
void at86rf23xSetTxPower(unsigned int txPower);
void at86rf23xTxData(unsigned char *data, unsigned char size);

unsigned char at86rf23xReadEd(void);

void at86rf23xTransmitFrame(unsigned char data);

void at86rf23xSetRxState(unsigned char state);

void at86rf23xProcess(void);

void at86rf23xInterruptHandler(void);

unsigned char *at86rf23xGetData(void);
unsigned char at86rf23xDataAvailable(void);

void at86rf23xSetClkmFreq(unsigned char frequency);

void at86rf23xEncryptData(unsigned char *data, unsigned char *key);
void at86rf23xDecryptData(unsigned char *data, unsigned char *key);

#define AT86RF23XMAX_PAYLOAD_SIZE 128
#define AT86RF23XAES_BLOCK_SIZE 16

#define AT86RF23XCMD_READ_REG 0x80
#define AT86RF23XCMD_WRITE_REG 0xC0
#define AT86RF23XCMD_READ_FRAME 0x20
#define AT86RF23XCMD_WRITE_FRAME 0x60
#define AT86RF23XCMD_READ_SRAM 0x00
#define AT86RF23XCMD_WRITE_SRAM 0x40

#define AT86RF23XTRX_CMD_NOP 0
#define AT86RF23XTRX_CMD_TX_START 2
#define AT86RF23XTRX_CMD_FORCE_TRX_OFF 3
#define AT86RF23XTRX_CMD_FORCE_PLL_ON 4
#define AT86RF23XTRX_CMD_RX_ON 6
#define AT86RF23XTRX_CMD_TRX_OFF 8
#define AT86RF23XTRX_CMD_PLL_ON 9
#define AT86RF23XTRX_CMD_RX_AACK_ON 22
#define AT86RF23XTRX_CMD_TX_ARET_ON 25

#define AT86RF23XCLKM_OFF 0
#define AT86RF23XCLKM_1MHZ 1
#define AT86RF23XCLKM_2MHZ 2
#define AT86RF23XCLKM_4MHZ 3
#define AT86RF23XCLKM_8MHZ 4
#define AT86RF23XCLKM_16MHZ 5
#define AT86RF23XCLKM_250KHZ 6
#define AT86RF23XCLKM_62KHZ 7

#define AT86RF23XTRX_STATUS_P_ON 0
#define AT86RF23XTRX_STATUS_BUSY_RX 1
#define AT86RF23XTRX_STATUS_BUSY_TX 2
#define AT86RF23XTRX_STATUS_RX_ON 6
#define AT86RF23XTRX_STATUS_TRX_OFF 8
#define AT86RF23XTRX_STATUS_PLL_ON 9
#define AT86RF23XTRX_STATUS_SLEEP 15
#define AT86RF23XTRX_STATUS_BUSY_RX_AACK 17
#define AT86RF23XTRX_STATUS_BUSY_TX_ARET 18
#define AT86RF23XTRX_STATUS_RX_AACK_ON 22
#define AT86RF23XTRX_STATUS_TX_ARET_ON 25
#define AT86RF23XTRX_STATUS_RX_ON_NOCLK 28
#define AT86RF23XTRX_STATUS_RX_AACK_ON_NOCLK 29
#define AT86RF23XTRX_STATUS_BUSY_RX_AACK_NOCLK 30
#define AT86RF23XTRX_STATUS_STATE_TRANSITION_IN_PROGRESS 31
#define AT86RF23XTRX_STATUS_TRX_STATUS_MASK 0x1f

#define AT86RF23XPLL_LOCK_MASK (1 << 0)
#define AT86RF23XPLL_UNLOCK_MASK (1 << 1)
#define AT86RF23XRX_START_MASK (1 << 2)
#define AT86RF23XTRX_END_MASK (1 << 3)
#define AT86RF23XTRX_UR_MASK (1 << 6)
#define AT86RF23XBAT_LOW_MASK (1 << 7)

#define AT86RF23XREG_TRX_STATUS 0x01
#define AT86RF23XREG_TRX_STATE 0x02
#define AT86RF23XREG_TRX_CTRL_0 0x03
#define AT86RF23XREG_TRX_CTRL_1 0x04
#define AT86RF23XREG_PHY_TX_PWR 0x05
#define AT86RF23XREG_PHY_RSSI 0x06
#define AT86RF23XREG_PHY_ED_LEVEL 0x07
#define AT86RF23XREG_PHY_CC_CCA 0x08
#define AT86RF23XREG_CCA_THRESH 0x09
#define AT86RF23XREG_RX_CTRL 0x0A
#define AT86RF23XREG_TRX_CTRL_2 0x0C
#define AT86RF23XREG_ANT_DIV 0x0D
#define AT86RF23XREG_IRQ_MASK 0x0E
#define AT86RF23XREG_IRQ_STATUS 0x0F
#define AT86RF23XREG_VREG_CTRL 0x10
#define AT86RF23XREG_BATMON 0x11
#define AT86RF23XREG_XOSC_CTRL 0x12
#define AT86RF23XREG_XAH_CTRL_1 0x17
#define AT86RF23XREG_PLL_CF 0x1a
#define AT86RF23XREG_PLL_DCU 0x1b
#define AT86RF23XREG_PART_NUM 0x1c
#define AT86RF23XREG_VERSION_NUM 0x1d
#define AT86RF23XREG_MAN_ID_0 0x1e
#define AT86RF23XREG_MAN_ID_1 0x1f
#define AT86RF23XREG_SHORT_ADDR_0 0x20
#define AT86RF23XREG_SHORT_ADDR_1 0x21
#define AT86RF23XREG_PAN_ID_0 0x22
#define AT86RF23XREG_PAN_ID_1 0x23
#define AT86RF23XREG_IEEE_ADDR_0 0x24
#define AT86RF23XREG_IEEE_ADDR_1 0x25
#define AT86RF23XREG_IEEE_ADDR_2 0x26
#define AT86RF23XREG_IEEE_ADDR_3 0x27
#define AT86RF23XREG_IEEE_ADDR_4 0x28
#define AT86RF23XREG_IEEE_ADDR_5 0x29
#define AT86RF23XREG_IEEE_ADDR_6 0x2a
#define AT86RF23XREG_IEEE_ADDR_7 0x2b
#define AT86RF23XREG_CSMA_SEED_0 0x2d
#define AT86RF23XREG_CSMA_SEED_1 0x2e

#define AT86RF23XIRQ_STATE_BAT_LOW 0x80
#define AT86RF23XIRQ_STATE_TRX_UR 0x40
#define AT86RF23XIRQ_STATE_TRX_END 0x08
#define AT86RF23XIRQ_STATE_RX_START 0x04
#define AT86RF23XIRQ_STATE_PLL_UNLOCK 0x02
#define AT86RF23XIRQ_STATE_PLL_LOCK 0x01

#define AT86RF23XREG_AES_STATUS 0x82
#define AT86RF23XREG_AES_CTRL 0x83
#define AT86RF23XREG_AES_DATA_START 0x84
#define AT86RF23XREG_AES_DATA_ADDR_END 0x93
#define AT86RF23XREG_AES_CTRL_MIRROR 0x94

#define AT86RF23XAES_CTRL_REQUEST 0x07
#define AT86RF23XAES_CTRL_MODE 0x04
#define AT86RF23XAES_CTRL_DIR 0x03

#define AT86RF23XAES_CTRL_MODE_ECB 0x00
#define AT86RF23XAES_CTRL_MODE_KEY 0x01
#define AT86RF23XAES_CTRL_MODE_CBC 0x02
#endif