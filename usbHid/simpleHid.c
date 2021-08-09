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
* Description: USB HID API provider
*/
#include <simpleHid.h>
#include <nimolib.h>

extern void usbHidProcess(uint8_t *req); /*This MUST be provided by the application*/
volatile unsigned char usbSendDirty = 0;
static uint8_t app_request_buffer[USB_BUFFER_SIZE];

#if defined(__SAMR21) || defined(__SAMD21)
#include "sam.h"
#include "atsamd21/usb_descriptors.c"
#include "atsamd21/usb_std.c"
#include "atsamd21/usbHid.c"

#define SIMPLEHID_EP_TO_HOST USB_IN_ENDPOINT
#define SIMPLEHID_EP_FROM_HOST USB_OUT_ENDPOINT
#elif defined(__NUVO_M032K)
#include "M031Series.h"

#include <string.h>
#include <stdio.h>
#include "NuMicro.h"
#include "m032/hid_transfer.h"
#include "m032/usbd.h"
#include "m032/usbd.c"
#include "m032/descriptors.c"
#include "m032/hid_transfer.c"

#define SIMPLEHID_EP_TO_HOST EP_INPUT
#define SIMPLEHID_EP_FROM_HOST EP_OUTPUT

#endif

//-----------------------------------------------------------------------------

void usbSend(uint8_t *data, int size)
{
    usb_send(SIMPLEHID_EP_TO_HOST, data, size);
}

void usbSendWait(uint8_t *data, int size)
{
    usb_send(SIMPLEHID_EP_TO_HOST, data, size);
    usbSendDirty = 1;

#if defined(__SAMR21) || defined(__SAMD21)
    while (usbSendDirty)
        usbTask();
#else
    while (usbSendDirty);
#endif
}

__attribute__((weak)) void usbHidProcess(uint8_t *req)
{
    (void) req;

}

#if defined(__SAMR21) || defined(__SAMD21)
__attribute__((weak)) void usb_configuration_callback(int config)
{
    usb_recv(USB_IN_ENDPOINT, app_request_buffer, sizeof(app_request_buffer));
    (void)config;
}

__attribute__((weak)) void usb_send_callback(int ep)
{
    usbSendDirty = 0;
    (void)ep;
}

__attribute__((weak)) void usb_recv_callback(int ep)
{
    if (USB_IN_ENDPOINT == ep)
    {
        usb_recv(USB_IN_ENDPOINT, app_request_buffer, sizeof(app_request_buffer));
        usbHidProcess(app_request_buffer);
    }
}
#endif