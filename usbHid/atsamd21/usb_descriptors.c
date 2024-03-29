
/*
 * Copyright (c) 2016, Alex Taradov <alex@taradov.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
* File: usb_descriptors.c
* Description: Updated to allow overrride of config from nimolib.h
*/
/*- Includes ----------------------------------------------------------------*/
#include <stdalign.h>
#include <nimolib.h>
#include "usbHid.h"
#include "usb_descriptors.h"

#ifndef USB_VID
#error USB_VID must be defined in nimolib.h
#endif

#ifndef USB_PID
#error USB_PID must be defined in nimolib.h
#endif

#define USB_HID_CONFIG_VID USB_VID
#define USB_HID_CONFIG_PID USB_PID

#ifndef USB_HID_CONFIG

#define USB_HID_CONFIG_MANUFACTURER "Nimolib"
#define USB_HID_CONFIG_PRODUCT "Generic HID"
#endif

/*- Variables ---------------------------------------------------------------*/
const alignas(4) usb_device_descriptor_t usb_device_descriptor =
{
    .bLength = sizeof(usb_device_descriptor_t),
    .bDescriptorType = USB_DEVICE_DESCRIPTOR,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = 64,
    .idVendor = USB_HID_CONFIG_VID,
    .idProduct = USB_HID_CONFIG_PID,
    .bcdDevice = 0x0100,
    .iManufacturer = USB_STR_MANUFACTURER,
    .iProduct = USB_STR_PRODUCT,
    .iSerialNumber = 0,
    .bNumConfigurations = 1,
};

const alignas(4) usb_configuration_hierarchy_t usb_configuration_hierarchy =
{
    .configuration =
    {
        .bLength = sizeof(usb_configuration_descriptor_t),
        .bDescriptorType = USB_CONFIGURATION_DESCRIPTOR,
        .wTotalLength = sizeof(usb_configuration_hierarchy_t),
        .bNumInterfaces = 1,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes = 0x80,
        .bMaxPower = 200, // 400 mA
    },

    .interface =
    {
        .bLength = sizeof(usb_interface_descriptor_t),
        .bDescriptorType = USB_INTERFACE_DESCRIPTOR,
        .bInterfaceNumber = 0,
        .bAlternateSetting = 0,
        .bNumEndpoints = 2,
        .bInterfaceClass = 0x03,
        .bInterfaceSubClass = 0x00,
        .bInterfaceProtocol = 0x00,
        .iInterface = 0,
    },

    .hid =
    {
        .bLength = sizeof(usb_hid_descriptor_t),
        .bDescriptorType = USB_HID_DESCRIPTOR,
        .bcdHID = 0x0111,
        .bCountryCode = 0,
        .bNumDescriptors = 1,
        .bDescriptorType1 = USB_HID_REPORT_DESCRIPTOR,
        .wDescriptorLength = sizeof(usb_hid_report_descriptor),
    },

    .ep_in =
    {
        .bLength = sizeof(usb_endpoint_descriptor_t),
        .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
        .bEndpointAddress = USB_IN_ENDPOINT | 1,
        .bmAttributes = USB_INTERRUPT_ENDPOINT,
        .wMaxPacketSize = 64,
        .bInterval = 1,
    },

    .ep_out =
    {
        .bLength = sizeof(usb_endpoint_descriptor_t),
        .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
        .bEndpointAddress = USB_OUT_ENDPOINT | 2,
        .bmAttributes = USB_INTERRUPT_ENDPOINT,
        .wMaxPacketSize = 64,
        .bInterval = 1,
    },
};

const alignas(4) uint8_t usb_hid_report_descriptor[33] =
{
    0x06, 0x00, 0xff, // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x01,		  // Usage (0x01)
    0xa1, 0x01,		  // Collection (Application)
    0x15, 0x00,		  //   Logical Minimum (0)
    0x26, 0xff, 0x00, //   Logical Maximum (255)
    0x75, 0x08,		  //   Report Size (8)
    0x95, 0x40,		  //   Report Count (64)
    0x09, 0x01,		  //   Usage (0x01)
    0x81, 0x02,		  //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x40,		  //   Report Count (64)
    0x09, 0x01,		  //   Usage (0x01)
    0x91, 0x02,		  //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x01,		  //   Report Count (1)
    0x09, 0x01,		  //   Usage (0x01)
    0xb1, 0x02,		  //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xc0,			  // End Collection
};

const alignas(4) usb_string_descriptor_zero_t usb_string_descriptor_zero =
{
    .bLength = sizeof(usb_string_descriptor_zero_t),
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .wLANGID = 0x0409, // English (United States)
};

const char *usb_strings[] =
{
    [USB_STR_MANUFACTURER] = USB_HID_CONFIG_MANUFACTURER,
    [USB_STR_PRODUCT] = USB_HID_CONFIG_PRODUCT,
};

alignas(4) uint8_t usb_string_descriptor_buffer[64];
