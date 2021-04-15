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

void xn297lInit(void);
void xn297lEnRxMode(void);
void xn297lReadStatus(void);
void xn297lSetChannel(uint8_t rfChannel);
void xn297lSetDataRate(uint8_t dataRate);
void xn297lSetTxPower(uint8_t power);
uint8_t xn297lTransmitData(uint8_t *data, uint8_t len);
int8_t xn297lReceiveData(uint8_t *data, uint8_t bufMaxLen);

/*Commands */

#define XN297L_CMD_READ_REG 0x00
#define XN297L_CMD_WRITE_REG 0x20
#define XN297L_CMD_RX_PAYLOAD 0x61
#define XN297L_CMD_TX_PAYLOAD 0xA0

/*Registers*/

#define XN297L_REG_CONFIG 0x00
#define XN297L_REG_EN_AA 0x01

#define XN297L_REG_SETUP_AW 0x03
#define XN297L_REG_SETUP_RETR 0x04

#define XN297L_REG_RFSETUP 0x06
#define XN297L_REG_STATUS 0x07

#define XN297L_REG_RX_PW_P0 0x11

#define XN297L_REG_DYNPD 0x1C
#define XN297L_REG_FEATURE 0x1D



#define XN297L_DATARATE_1M 0
#define XN297L_DATARATE_2M 1
#define XN297L_DATARATE_250k 3

#endif