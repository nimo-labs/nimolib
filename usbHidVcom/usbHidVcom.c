/*
* Copyright 2024 NimoLabs Ltd.
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
* File: usbHidVcom.c
* Description: USB Dual Role HID and VCOM API provider
*/
#include <nimolib.h>
#include <stdint.h>

#if defined(__NUVO_M032K)
#include "M031Series.h"

#include <string.h>
#include <stdio.h>
#include "NuMicro.h"
#include "m032/m032.h"
#include "m032/usbd.h"
#include "m032/usbd.c"
#include "m032/descriptors.c"
#include "m032/m032.c"

#warning these are from the demo and should be removed once this driver has been re-written to match usbVcom
#define RX_BUFSIZE           512 /* RX buffer size */
#define TX_BUFSIZE           512 /* RX buffer size */
#define TX_FIFO_SIZE         16  /* TX Hardware FIFO size */

STR_VCOM_LINE_CODING gLineCoding = {115200, 0, 0, 8};   /* Baud rate : 115200    */
volatile uint8_t comRbuf[RX_BUFSIZE];
volatile uint16_t comRbytes = 0;
volatile uint16_t comRhead = 0;
volatile uint16_t comRtail = 0;

volatile uint8_t comTbuf[TX_BUFSIZE];
volatile uint16_t comTbytes = 0;
volatile uint16_t comThead = 0;
volatile uint16_t comTtail = 0;

uint8_t gRxBuf[64] = {0};
uint8_t *gpu8RxBuf = 0;
uint32_t gu32RxSize = 0;
uint32_t gu32TxSize = 0;

volatile int8_t gi8BulkOutReady = 0;

#elif
#error usbHidVcom: Processor not supported

#endif

//-----------------------------------------------------------------------------

