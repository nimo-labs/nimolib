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
* File: uart.c
* Description: ATSAMD UART API provider
*/

/*
* ATSAM21D nimolib.h example:
*
* #define UART_CHAN0 0
* #define UART_CHAN0_SERCOM 3
* #define UART_CHAN0_IRQ void irq_handler_sercom3(void)
* #define UART_CHAN0_FIFO_LEN 10
* #define UART_CHAN0_PORT SAM_GPIO_PORTA
* #define UART_CHAN0_RX_PIN 25
* #define UART_CHAN0_TX_PIN 24
* #define UART_CHAN0_RX_PAD 3
* #define UART_CHAN0_TX_PAD 1
* #define UART_CHAN0_PERHIPH_RX_MUX SAM_GPIO_PMUX_C
* #define UART_CHAN0_PERHIPH_TX_MUX SAM_GPIO_PMUX_C
*
*
* M032K nimolib.h example:
*
* #define UART_CHAN0 0
* #define UART_CHAN0_SERCOM 3
* #define UART_CHAN0_IRQ void irq_handler_sercom3(void)
* #define UART_CHAN0_FIFO_LEN 10
* #define UART_CHAN0_PORT SAM_GPIO_PORTA
* #define UART_CHAN0_RX_PIN 25
* #define UART_CHAN0_TX_PIN 24
*/

#include <nimolib.h>
#include "uart.h"

#ifdef UART_CHAN0
unsigned char uartChan0Fifo[UART_CHAN0_FIFO_LEN];
unsigned int uartChan0FifoWrite = 0;
unsigned int uartChan0FifoRead = 0;
#endif
#ifdef UART_CHAN1
unsigned char uartChan1Fifo[UART_CHAN1_FIFO_LEN];
unsigned int uartChan1FifoWrite = 0;
unsigned int uartChan1FifoRead = 0;
#endif
#ifdef UART_CHAN2
unsigned char uartChan2Fifo[UART_CHAN2_FIFO_LEN];
unsigned int uartChan2FifoWrite = 0;
unsigned int uartChan2FifoRead = 0;
#endif
#ifdef UART_CHAN3
unsigned char uartChan3Fifo[UART_CHAN3_FIFO_LEN];
unsigned int uartChan3FifoWrite = 0;
unsigned int uartChan3FifoRead = 0;
#endif
#ifdef UART_CHAN4
unsigned char uartChan4Fifo[UART_CHAN4_FIFO_LEN];
unsigned int uartChan4FifoWrite = 0;
unsigned int uartChan4FifoRead = 0;
#endif
#ifdef UART_CHAN5
unsigned char uartChan5Fifo[UART_CHAN5_FIFO_LEN];
unsigned int uartChan5FifoWrite = 0;
unsigned int uartChan5FifoRead = 0;
#endif

unsigned char uartIsData(unsigned char uart)
{
    switch (uart)
    {
#ifdef UART_CHAN0
    case UART_CHAN0:
        if (uartChan0FifoRead != uartChan0FifoWrite)
            return 1;
        else
            return 0;
        break;
#endif
#ifdef UART_CHAN1
    case UART_CHAN1:
        if (uartChan1FifoRead != uartChan1FifoWrite)
            return 1;
        else
            return 0;
        break;
#endif
#ifdef UART_CHAN2
    case UART_CHAN2:
        if (uartChan2FifoRead != uartChan2FifoWrite)
            return 1;
        else
            return 0;
        break;
#endif
#ifdef UART_CHAN3
    case UART_CHAN3:
        if (uartChan3FifoRead != uartChan3FifoWrite)
            return 1;
        else
            return 0;
        break;
#endif
#ifdef UART_CHAN4
    case UART_CHAN4:
        if (uartChan4FifoRead != uartChan4FifoWrite)
            return 1;
        else
            return 0;
        break;
#endif
#ifdef UART_CHAN5
    case UART_CHAN5:
        if (uartChan5FifoRead != uartChan5FifoWrite)
            return 1;
        else
            return 0;
        break;
#endif
    default:
        return 0;
        break;
    }
}

unsigned char uartGetData(unsigned char uart)
{
    unsigned char tempData;
    switch (uart)
    {
#ifdef UART_CHAN0
    case UART_CHAN0:
        if (uartChan0FifoRead != uartChan0FifoWrite)
        {
            tempData = uartChan0Fifo[uartChan0FifoRead];
            if (uartChan0FifoRead < UART_CHAN0_FIFO_LEN)
                uartChan0FifoRead++;
            else
                uartChan0FifoRead = 0;
            return tempData;
        }
        else
            return 0;
        break;
#endif
#ifdef UART_CHAN1
    case UART_CHAN1:
        if (uartChan1FifoRead != uartChan1FifoWrite)
        {
            tempData = uartChan1Fifo[uartChan1FifoRead];
            if (uartChan1FifoRead < UART_CHAN1_FIFO_LEN)
                uartChan1FifoRead++;
            else
                uartChan1FifoRead = 0;
            return tempData;
        }
        else
            return 0;
        break;
#endif
#ifdef UART_CHAN2
    case UART_CHAN2:
        if (uartChan2FifoRead != uartChan2FifoWrite)
        {
            tempData = uartChan2Fifo[uartChan2FifoRead];
            if (uartChan2FifoRead < UART_CHAN2_FIFO_LEN)
                uartChan2FifoRead++;
            else
                uartChan2FifoRead = 0;
            return tempData;
        }
        else
            return 0;
        break;
#endif
#ifdef UART_CHAN3
    case UART_CHAN3:
        if (uartChan3FifoRead != uartChan3FifoWrite)
        {
            tempData = uartChan3Fifo[uartChan3FifoRead];
            if (uartChan3FifoRead < UART_CHAN3_FIFO_LEN)
                uartChan3FifoRead++;
            else
                uartChan3FifoRead = 0;
            return tempData;
        }
        else
            return 0;
        break;
#endif
#ifdef UART_CHAN4
    case UART_CHAN4:
        if (uartChan4FifoRead != uartChan4FifoWrite)
        {
            tempData = uartChan4Fifo[uartChan4FifoRead];
            if (uartChan4FifoRead < UART_CHAN4_FIFO_LEN)
                uartChan4FifoRead++;
            else
                uartChan4FifoRead = 0;
            return tempData;
        }
        else
            return 0;
        break;
#endif
#ifdef UART_CHAN5
    case UART_CHAN5:
        if (uartChan5FifoRead != uartChan5FifoWrite)
        {
            tempData = uartChan5Fifo[uartChan5FifoRead];
            if (uartChan5FifoRead < UART_CHAN5_FIFO_LEN)
                uartChan5FifoRead++;
            else
                uartChan5FifoRead = 0;
            return tempData;
        }
        else
            return 0;
        break;
#endif
    default:
        return 0;
        break;
    }
}

#if defined(__SAMR21)
#include "atsamd21.c"
#elif defined(__SAMD21)
#include "atsamd21.c"
#elif defined(__NUVO_M032K)
#include "m032.c"
#endif