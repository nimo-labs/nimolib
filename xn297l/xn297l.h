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

#define XN297L_CMD_READ_REG 0x00
#define XN297L_CMD_WRITE_REG 0x20
#define XN297L_CMD_RX_PAYLOAD 0x60
#define XN297L_CMD_TX_PAYLOAD 0xA0

#define XN297L_REG_CONFIG 0x00
#define XN297L_REG_EN_AA 0x01

#endif