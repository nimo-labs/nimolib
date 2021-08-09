/*
 * This file is part of the nimolib distribution (https://github.com/nimo-labs/nimolib).
 * Copyright (c) 2020 Nimolabs Ltd. www.nimo.uk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
* File: USBHid.h
* Description: ATSAMD USB HID device driver
*/

#ifndef _USB_H_
#define _USB_H_

/*- Includes ----------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "usb_descriptors.h"

/*- Prototypes --------------------------------------------------------------*/
void usbInit(void);
void usb_attach(void);
void usb_detach(void);
void usb_reset_endpoint(int ep, int dir);
void usb_configure_endpoint(usb_endpoint_descriptor_t *ep_desc);
bool usb_endpoint_configured(int ep, int dir);
int usb_endpoint_get_status(int ep, int dir);
void usb_endpoint_set_feature(int ep, int dir);
void usb_endpoint_clear_feature(int ep, int dir);
void usb_set_address(int address);
void usb_send(int ep, uint8_t *data, int size);
void usb_recv(int ep, uint8_t *data, int size);
void usb_control_send_zlp(void);
void usb_control_stall(void);
void usb_control_send(uint8_t *data, int size);
void usbTask(void);

void usb_configuration_callback(int config);
void usb_recv_callback(int ep);
void usb_send_callback(int ep);

#define USB_EP_SEND           1
#define USB_EP_RECV           2

#endif // _USB_H_
