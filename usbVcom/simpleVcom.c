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
#include <nimolib.h>
#include <stdint.h>

extern void usbHidProcess(uint8_t *req); /*This MUST be provided by the application*/
volatile unsigned char usbSendDirty = 0;
//static uint8_t app_request_buffer[USB_BUFFER_SIZE];

#if defined(__SAMR21) || defined(__SAMD21)
#include "sam.h"
#include "atsamd21/usb_descriptors.c"
#include "atsamd21/usb_std.c"
#include "atsamd21/usb_hid.c"
#elif defined(__NUVO_M032K)
#include "M031Series.h"

#include <string.h>
#include <stdio.h>
#include "NuMicro.h"
#include "m032/vcom_serial.h"
#include "m032/usbd.h"
#include "m032/usbd.c"
#include "m032/descriptors.c"
#include "m032/vcom_serial.c"

volatile uint8_t comRbuf[SIMPLE_VCOM_RX_BUFSIZE];
volatile uint16_t comRbytes = 0;
volatile uint16_t comRhead = 0;
volatile uint16_t comRtail = 0;

volatile uint8_t comTbuf[SIMPLE_VCOM_TX_BUFSIZE];
volatile uint16_t comTbytes = 0;
volatile uint16_t comThead = 0;
volatile uint16_t comTtail = 0;

uint8_t gRxBuf[64] = {0};
uint8_t *gpu8RxBuf = 0;
uint32_t gu32RxSize = 0;
uint32_t gu32TxSize = 0;

volatile int8_t gi8BulkOutReady = 0;

STR_VCOM_LINE_CODING gLineCoding = {115200, 0, 0, 8};   /* Baud rate: 115200     */
/* Stop bit : 1 Stops Bit*/
/* parity   : None       */
/* data bits: 8 Bits     */
uint16_t gCtrlSignal = 0;     /* BIT0: DTR(Data Terminal Ready) , BIT1: RTS(Request To Send) */

void VCOM_TransferData(void)
{
    int32_t i, i32Len;

    /* Transmit Data */
    /* Check whether USB is ready for next packet or not*/
    if(gu32TxSize == 0)
    {
        /* Check whether we have new COM Rx data to send to USB or not */
        if(comRbytes)
        {
            i32Len = comRbytes;
            if(i32Len > EP2_MAX_PKT_SIZE)
                i32Len = EP2_MAX_PKT_SIZE;

            // for(i = 0; i < i32Len; i++)
            // {
            //     gRxBuf[i] = comRbuf[comRhead++];
            //     if(comRhead >= SIMPLE_VCOM_RX_BUFSIZE)
            //         comRhead = 0;
            // }

            __set_PRIMASK(1);
            comRbytes -= i32Len;
            __set_PRIMASK(0);

            gu32TxSize = i32Len;
            USBD_MemCopy((uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP2)), (uint8_t *)gRxBuf, i32Len);
            USBD_SET_PAYLOAD_LEN(EP2, i32Len);
        }
        else
        {
            /* Prepare a zero packet if previous packet size is EP2_MAX_PKT_SIZE and
               no more data to send at this moment to note Host the transfer has been done */
            i32Len = USBD_GET_PAYLOAD_LEN(EP2);
            if(i32Len == EP2_MAX_PKT_SIZE)
                USBD_SET_PAYLOAD_LEN(EP2, 0);
        }
    }

    /* Process the Bulk out data when bulk out data is ready. */
    if(gi8BulkOutReady && (gu32RxSize <= SIMPLE_VCOM_TX_BUFSIZE - comTbytes))
    {
        for(i = 0; i < gu32RxSize; i++)
        {
            comTbuf[comTtail++] = gpu8RxBuf[i];
            if(comTtail >= SIMPLE_VCOM_TX_BUFSIZE)
                comTtail = 0;
        }

        __set_PRIMASK(1);
        comTbytes += gu32RxSize;
        __set_PRIMASK(0);

        gu32RxSize = 0;
        gi8BulkOutReady = 0; /* Clear bulk out ready flag */

        /* Ready to get next BULK out */
        USBD_SET_PAYLOAD_LEN(EP3, EP3_MAX_PKT_SIZE);
    }

    /* Rx data? */
    /* Process the software Tx FIFO */
    // if(comTbytes)
    // {
    //     /* Check if Tx is working */
    //     if((UART0->INTEN & UART_INTEN_THREIEN_Msk) == 0)
    //     {
    //         /* Send one bytes out */
    //         UART0->DAT = comTbuf[comThead++];

    //         if (comThead >= SIMPLE_VCOM_TX_BUFSIZE)
    //             comThead = 0;

    //         __set_PRIMASK(1);
    //         comTbytes--;
    //         __set_PRIMASK(0);

    //         /* Enable Tx Empty Interrupt. (Trigger first one) */
    //         UART_ENABLE_INT(UART0, UART_INTEN_THREIEN_Msk);
    //     }
    // }
}

#endif

//-----------------------------------------------------------------------------

