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
* File: usbHidVcom.h
* Description: USB Dual Role HID and VCOM API provider
*/


#if defined(__SAMR21) || defined(__SAMD21)
#include <usbHid.h>
#elif defined(__NUVO_M032K)
#include "M031Series.h"
#include <stdint.h>
#include "m032/usbd.h"
#include "m032/m032.h"

#define USB_VCOM 0

#endif

/********** Definitions to be provided by nimolib.h *************
 *
 * USB_BUFFER_SIZE
 *
 */

void usbSendWait(int ep, uint8_t *data, int size);
uint8_t vcomSend(uint8_t *data, uint32_t size);