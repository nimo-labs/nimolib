/******************************************************************************
 * @file     hid_mouse.c
 * @version  V1.00
 * $Revision: 11 $
 * $Date: 18/07/18 4:54p $
 * @brief    M031 series USBD driver Sample file
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
/*!<Includes */


uint8_t volatile g_u8EP2Ready = 0;
uint8_t volatile g_u8Suspend = 0;
uint8_t g_u8Idle = 0, g_u8Protocol = 0;

static uint8_t  g_hidRecvBuf[USB_BUFFER_SIZE] = {0};    /* HID Receive buffer*/
static uint8_t  g_hidSendBuf[USB_BUFFER_SIZE] = {0};    /* HID Send buffer*/

void HID_ClassRequest(void);
void EP2_Handler(void);
void EP3_Handler(void);

void USBD_IRQHandler(void)
{
    uint32_t volatile u32IntSts = USBD_GET_INT_FLAG();
    uint32_t volatile u32State = USBD_GET_BUS_STATE();

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

        if (u32IntSts & USBD_INTSTS_EP2)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP2);
            /* Interrupt IN */
            EP2_Handler();
        }

        if (u32IntSts & USBD_INTSTS_EP3)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP3);
            /* Interrupt OUT */
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
}

void EP2_Handler(void)  /* Interrupt IN (to host) handler */
{
    uint8_t *ptr; /*EP ptr*/
    /* Prepare the data for next HID IN transfer */
    ptr = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP2));
    USBD_MemCopy(ptr, (void *)g_hidSendBuf, USB_BUFFER_SIZE);
    USBD_SET_PAYLOAD_LEN(EP2, USB_BUFFER_SIZE);
    usbSendDirty = 0;
}

void EP3_Handler(void)  /* Interrupt OUT (from host) handler */
{
    uint8_t *ptr;
    /* Interrupt OUT */
    ptr = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP3));
    USBD_MemCopy(g_hidRecvBuf, ptr, USB_BUFFER_SIZE);
    USBD_SET_PAYLOAD_LEN(EP3, USB_BUFFER_SIZE);
    usbHidProcess(g_hidRecvBuf); /*This MUST be provided by the application*/
}


/*--------------------------------------------------------------------------*/
/**
  * @brief  USBD Endpoint Config.
  * @param  None.
  * @retval None.
  */
void HID_Init(void)
{
    /* Switch USB clock source to HIRC & USB Clock = HIRC / 1 */
//    CLK_SetModuleClock(USBD_MODULE, CLK_CLKSEL0_USBDSEL_HIRC, CLK_CLKDIV0_USB(1));

    /* Enable USB clock */
    CLK->APBCLK0 |= CLK_APBCLK0_USBDCKEN_Msk;
    //CLK_EnableModuleClock(USBD_MODULE);

    /* Update System Core Clock */
    //SystemCoreClockUpdate();

    /* Setup USB device */
    USBD_Open(&gsInfo, HID_ClassRequest, NULL);

    /* Init setup packet buffer */
    /* Buffer range for setup packet -> [0 ~ 0x7] */
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
    /* EP2 ==> Interrupt IN endpoint, address 1 */
    USBD_CONFIG_EP(EP2, USBD_CFG_EPMODE_IN | INT_IN_EP_NUM);
    /* Buffer range for EP2 */
    USBD_SET_EP_BUF_ADDR(EP2, EP2_BUF_BASE);

    /* EP3 ==> Interrupt OUT endpoint, address 2 */
    USBD_CONFIG_EP(EP3, USBD_CFG_EPMODE_OUT | INT_OUT_EP_NUM);
    /* Buffer range for EP3 */
    USBD_SET_EP_BUF_ADDR(EP3, EP3_BUF_BASE);
    /* trigger to receive OUT data */
    USBD_SET_PAYLOAD_LEN(EP3, EP3_MAX_PKT_SIZE);

    /* Start USB device */
    USBD_Start();

    NVIC_EnableIRQ(USBD_IRQn);
    USBD->INTEN = USBD_INTEN_USBIEN_Msk;
}

void HID_ClassRequest(void)
{
    uint8_t buf[8];
    USBD_GetSetupPacket(buf);

    if(buf[0] & 0x80)    /* request data transfer direction */
    {
        /* Device to host */
        switch(buf[1])
        {
        case GET_IDLE:
        {
            USBD_SET_PAYLOAD_LEN(EP1, buf[6]);
            /* Data stage */
            USBD_PrepareCtrlIn(&g_u8Idle, buf[6]);
            /* Status stage */
            USBD_PrepareCtrlOut(0, 0);
            break;
        }
        case GET_PROTOCOL:
        {
            USBD_SET_PAYLOAD_LEN(EP1, buf[6]);
            /* Data stage */
            USBD_PrepareCtrlIn(&g_u8Protocol, buf[6]);
            /* Status stage */
            USBD_PrepareCtrlOut(0, 0);
            break;
        }
        case GET_REPORT:
//             {
//                 break;
//             }
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
        switch(buf[1])
        {
        case SET_REPORT:
        {
            if(buf[3] == 3)
            {
                /* Request Type = Feature */
                USBD_SET_DATA1(EP1);
                USBD_SET_PAYLOAD_LEN(EP1, 0);
            }
            break;
        }
        case SET_IDLE:
        {
            g_u8Idle = buf[3];
            /* Status stage */
            USBD_SET_DATA1(EP0);
            USBD_SET_PAYLOAD_LEN(EP0, 0);
            break;
        }
        case SET_PROTOCOL:
        {
            g_u8Protocol = buf[2];
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


void usbSend(uint8_t ep, uint8_t *data, uint32_t size)
{
    (void) ep;
    if(size <= USB_BUFFER_SIZE)
    {
        memcpy(g_hidSendBuf, data, size);
    }
    else
    {
        printf("usbSend Error\r\n");
    }
}
/***************************************************************/