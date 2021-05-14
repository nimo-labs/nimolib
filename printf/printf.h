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
* File: printf.h
* Description: Simple printf library
*/

/* This driver requires PRINTF_UART to be defined in nimolib.h
*
* Examples:
*
* #define PRINTF_UART UART_CHAN0 directs printf output to UART 0 as defined by the uart driver
*
* #define PRINTF_UART USB_VCOM directs printf output to to USB virtual com port driver
*/

#define PRINTF_USB_VCOM 99

int printf(const char *format, ...);
void printfOutputHex(char *marker, unsigned char *data, unsigned char dataLen);