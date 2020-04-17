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
* File: simpleHid.c
* Description: ATSAMD USB HID API provider
*/
#include <simpleHid.h>
#include <usbHid.h>
#include <nimolib.h>

//-----------------------------------------------------------------------------

extern void usbHidProcess(uint8_t *req); /*This MUST be provided by the application*/

volatile unsigned char usbSendDirty = 0;
static uint8_t app_request_buffer[USB_BUFFER_SIZE];


void usbSendWait(int ep, uint8_t *data, int size)
{
    usb_send(ep, data, size);
    usbSendDirty = 1;
    while (usbSendDirty)
        usbTask();
}

__attribute__((weak)) void usbHidProcess(uint8_t *req)
{
    (void) req;

}

__attribute__((weak)) void usb_configuration_callback(int config)
{
    usb_recv(USB_EP_RECV, app_request_buffer, sizeof(app_request_buffer));
    (void)config;
}

__attribute__((weak)) void usb_send_callback(int ep)
{
    usbSendDirty = 0;
    (void)ep;
}

__attribute__((weak)) void usb_recv_callback(int ep)
{
    if (USB_EP_RECV == ep)
    {
        usb_recv(USB_EP_RECV, app_request_buffer, sizeof(app_request_buffer));
        usbHidProcess(app_request_buffer);
    }
}