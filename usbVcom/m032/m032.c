/******************************************************************************
 * @file     vcom_serial.c
 * @version  V1.00
 * $Revision: 10 $
 * $Date: 18/07/18 4:46p $
 * @brief    M031 series USBD driver Sample file
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

/*!<Includes */
#include <string.h>
#include "NuMicro.h"
#include "usbd.h"

uint8_t volatile g_u8Suspend = 0;
static uint8_t txBuf[SIMPLE_VCOM_TX_BUFSIZE];
static volatile uint32_t txBufHead = 0;
static volatile uint32_t txBufTail = 0;
static volatile uint8_t txLock=0;

void usbTriggerSend(void);
__attribute__((weak)) void vcomRecv(uint8_t *data, uint32_t size);

/*--------------------------------------------------------------------------*/
void USBD_IRQHandler(void)
{
    uint32_t u32IntSts = USBD_GET_INT_FLAG();
    uint32_t u32State = USBD_GET_BUS_STATE();

    if (u32IntSts & USBD_INTSTS_FLDET)
    {
        /* Floating detect */
        USBD_CLR_INT_FLAG(USBD_INTSTS_FLDET);

        if (USBD_IS_ATTACHED())
        {
            /* USB Plug In */
            USBD_ENABLE_USB();
        }
        else
        {
            /* USB Un-plug */
            USBD_DISABLE_USB();
        }
    }

    if (u32IntSts & USBD_INTSTS_BUS)
    {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_BUS);

        if (u32State & USBD_STATE_USBRST)
        {
            /* Bus reset */
            USBD_ENABLE_USB();
            USBD_SwReset();
            g_u8Suspend = 0;
        }
        if (u32State & USBD_STATE_SUSPEND)
        {
            /* Enter power down to wait USB attached */
            g_u8Suspend = 1;

            /* Enable USB but disable PHY */
            USBD_DISABLE_PHY();
        }
        if (u32State & USBD_STATE_RESUME)
        {
            /* Enable USB and enable PHY */
            USBD_ENABLE_USB();
            g_u8Suspend = 0;
        }
    }

    if(u32IntSts & USBD_INTSTS_SOF)
    {
        /* Clear SOF flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_SOF);
    }

    if(u32IntSts & USBD_INTSTS_WAKEUP)
    {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_WAKEUP);
    }

    if (u32IntSts & USBD_INTSTS_USB)
    {
        extern uint8_t g_usbd_SetupPacket[];
        /* USB event */
        if (u32IntSts & USBD_INTSTS_SETUP)
        {
            /* Setup packet */
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_SETUP);

            /* Clear the data IN/OUT ready flag of control end-points */
            USBD_STOP_TRANSACTION(EP0);
            USBD_STOP_TRANSACTION(EP1);

            USBD_ProcessSetupPacket();
            if(g_usbd_SetupPacket[1] == SET_LINE_CODE)
            {
                USBD_SetStall(EP0);
            }
            if(g_usbd_SetupPacket[1] == SET_CONTROL_LINE_STATE)
            {
                USBD_SetStall(EP0);
            }
        }

        /* EP events */
        if (u32IntSts & USBD_INTSTS_EP0)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP0);
            /* control IN */
            USBD_CtrlIn();

        }

        if (u32IntSts & USBD_INTSTS_EP1)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP1);
            /* control OUT */
            USBD_CtrlOut();
        }
    }

    if (u32IntSts & USBD_INTSTS_EP2)
    {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_EP2);
        /* Bulk IN */
        EP2_Handler();
    }

    if (u32IntSts & USBD_INTSTS_EP3)
    {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_EP3);
        /* Bulk Out */
        EP3_Handler();
    }

    if (u32IntSts & USBD_INTSTS_EP4)
    {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_EP4);
    }

    if (u32IntSts & USBD_INTSTS_EP5)
    {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_EP5);
    }

    if (u32IntSts & USBD_INTSTS_EP6)
    {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_EP6);
    }

    if (u32IntSts & USBD_INTSTS_EP7)
    {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_EP7);
    }
}

void EP2_Handler(void)
{
    /*TX handler*/
    txLock=0;
    usbTriggerSend();
}


void EP3_Handler(void)
{
    uint32_t u32RxSize = USBD_GET_PAYLOAD_LEN(EP3);
    uint8_t * pu8RxBuf = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP3));
    /* RX Handler */
    vcomRecv(pu8RxBuf, u32RxSize);
    USBD_SET_PAYLOAD_LEN(EP3, EP3_MAX_PKT_SIZE);
}

void usbTriggerSend(void)
{
    uint8_t sendDataSize=0;

    if(0==txLock)
    {
        if(txBufHead != txBufTail)
        {
            if((txBufHead > txBufTail /*txBufHead hasn't wrapped yet*/))
            {
                sendDataSize = txBufHead-txBufTail;
                if(sendDataSize > 64)
                    sendDataSize = 64;
                USBD_MemCopy((uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP2)), &txBuf[txBufTail], sendDataSize);
                txBufTail+=sendDataSize;
            }
            else if((SIMPLE_VCOM_TX_BUFSIZE - txBufTail) >= 64) /*txBufHead has wrapped, but txBufTail is > 64 bytes from end of buf*/
            {
                sendDataSize = 64;
                USBD_MemCopy((uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP2)), &txBuf[txBufTail], sendDataSize);
                txBufTail+=sendDataSize;
            }
            else /*txBufHead has wrapped, but txBufTail is <= 64 bytes from end of buf*/
            {
                sendDataSize = SIMPLE_VCOM_TX_BUFSIZE - txBufTail;
                USBD_MemCopy((uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP2)), &txBuf[txBufTail], sendDataSize);
                txBufTail=0;
                if(txBufHead <= (uint8_t)(64-sendDataSize)) /*Unsent data <= 64 bytes*/
                {
                    USBD_MemCopy((uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP2))+sendDataSize, &txBuf[txBufTail], txBufHead);
                    txBufTail+=txBufHead;
                    sendDataSize += txBufHead;
                }
                else /*Unsent data > 64 bytes*/
                {
                    USBD_MemCopy((uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP2))+sendDataSize, &txBuf[txBufTail], 64-sendDataSize);
                    txBufTail+=64-sendDataSize;
                    sendDataSize =64;
                }
            }
            USBD_SET_PAYLOAD_LEN(EP2, sendDataSize);
            txLock=1;
        }
    }
}


/*--------------------------------------------------------------------------*/
/**
  * @brief  USBD Endpoint Config.
  * @param  None.
  * @retval None.
  */
void VCOM_Init(void)
{

    /* Enable USB clock */
    CLK->APBCLK0 |= CLK_APBCLK0_USBDCKEN_Msk;
    //CLK_EnableModuleClock(USBD_MODULE);

    /* Update System Core Clock */
    //SystemCoreClockUpdate();

    /* Setup USB device */
    USBD_Open(&gsInfo, NULL, NULL);
    /* Init setup packet buffer */
    /* Buffer for setup packet -> [0 ~ 0x7] */
    USBD->STBUFSEG = SETUP_BUF_BASE;

    /*****************************************************/
    /* EP0 ==> control IN endpoint, address 0 */
    USBD_CONFIG_EP(EP0, USBD_CFG_CSTALL | USBD_CFG_EPMODE_IN | 0);
    /* Buffer range for EP0 */
    USBD_SET_EP_BUF_ADDR(EP0, EP0_BUF_BASE);

    /* EP1 ==> control OUT endpoint, address 0 */
    USBD_CONFIG_EP(EP1, USBD_CFG_CSTALL | USBD_CFG_EPMODE_OUT | 0);
    /* Buffer range for EP1 */
    USBD_SET_EP_BUF_ADDR(EP1, EP1_BUF_BASE);

    /*****************************************************/
    /* EP2 ==> Bulk IN endpoint, address 1 */
    USBD_CONFIG_EP(EP2, USBD_CFG_EPMODE_IN | BULK_IN_EP_NUM);
    /* Buffer offset for EP2 */
    USBD_SET_EP_BUF_ADDR(EP2, EP2_BUF_BASE);

    /* EP3 ==> Bulk Out endpoint, address 2 */
    USBD_CONFIG_EP(EP3, USBD_CFG_EPMODE_OUT | BULK_OUT_EP_NUM);
    /* Buffer offset for EP3 */
    USBD_SET_EP_BUF_ADDR(EP3, EP3_BUF_BASE);
    /* trigger receive OUT data */
    USBD_SET_PAYLOAD_LEN(EP3, EP3_MAX_PKT_SIZE);

    /* EP4 ==> Interrupt IN endpoint, address 3 */
    USBD_CONFIG_EP(EP4, USBD_CFG_EPMODE_IN | INT_IN_EP_NUM);
    /* Buffer offset for EP4 ->  */
    USBD_SET_EP_BUF_ADDR(EP4, EP4_BUF_BASE);

    /* Start USB device */
    USBD_Start();

    NVIC_EnableIRQ(USBD_IRQn);
    USBD->INTEN = USBD_INTEN_USBIEN_Msk;
}


void VCOM_ClassRequest(void)
{
    uint8_t buf[8];

    USBD_GetSetupPacket(buf);

    if (buf[0] & 0x80)   /* request data transfer direction */
    {
        /* Device to host */
        switch (buf[1])
        {
        case GET_LINE_CODE:
        {
            if (buf[4] == 0)   /* VCOM-1 */
            {
                USBD_MemCopy((uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0)), (uint8_t *)&gLineCoding, 7);
            }
            /* Data stage */
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 7);
            /* Status stage */
            USBD_PrepareCtrlOut(0,0);
            break;
        }
        default:
        {
            /* Setup error, stall the device */
            USBD_SetStall(EP0);
            USBD_SetStall(EP1);
            break;
        }
        }
    }
    else
    {
        /* Host to device */
        switch (buf[1])
        {
        case SET_CONTROL_LINE_STATE:
        {
            if (buf[4] == 0)   /* VCOM-1 */
            {
                gCtrlSignal = buf[3];
                gCtrlSignal = (gCtrlSignal << 8) | buf[2];
                //printf("RTS=%d  DTR=%d\n", (gCtrlSignal0 >> 1) & 1, gCtrlSignal0 & 1);
            }

            /* Status stage */
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 0);
            break;
        }
        case SET_LINE_CODE:
        {
            if (buf[4] == 0) /* VCOM-1 */
                USBD_PrepareCtrlOut((uint8_t *)&gLineCoding, 7);

            /* Status stage */
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 0);

            break;
        }
        default:
        {
            /* Stall */
            /* Setup error, stall the device */
            USBD_SetStall(EP0);
            USBD_SetStall(EP1);
            break;
        }
        }
    }
}

__attribute__((weak)) void vcomRecv(uint8_t *data, uint32_t size)
{
    (void) data;
    (void) size;
}

uint8_t vcomSend(uint8_t *data, uint32_t size)
{
    if(size <= USB_BUFFER_SIZE)
    {
        uint16_t spaceLeft = (SIMPLE_VCOM_TX_BUFSIZE - txBufHead);
        if(size <= spaceLeft)
        {
            /*New data fits in buffer without wrapping*/
            memcpy(&txBuf[txBufHead], data, size);
            txBufHead+= size;
            if(txBufHead >= SIMPLE_VCOM_TX_BUFSIZE) /*Should never actually be true??*/
                txBufHead=0;
        }
        else
        {
            /*Fill buffer then wrap to accept remaining data*/
            memcpy(&txBuf[txBufHead], data, spaceLeft);
            txBufHead=0;
            memcpy(&txBuf[txBufHead], &data[spaceLeft], size-spaceLeft);
            txBufHead= size-spaceLeft;
        }
        usbTriggerSend();
        return 0;
    }
    else
        return 1;
}

int putchar(int c)
{
    uint8_t uiC = (uint8_t) c;
    vcomSend(&uiC, 1);
    return 0;
}