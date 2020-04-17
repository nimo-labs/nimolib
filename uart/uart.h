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
* File: uart.h
* Description: ATSAMD UART API provider
*/

#define UART_BAUD_9600 0

void uartInit(unsigned char uart, uint32_t baud);
unsigned char uartIsData(unsigned char uart);
/** returns a byte if there is data available or 0 if no data is available */
unsigned char uartGetData(unsigned char uart);
void uartTx(unsigned char uart, unsigned char data);
