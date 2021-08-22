/*
* Copyright 2021 NimoLabs Ltd.
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
* File: m032k.c
* Description: Nuvoton M032K UART device driver
*/

#include "M031Series.h"

#define ISR_CONCAT_(ID) SERCOM##ID##_Handler()
#define ISR_CONCAT(ID) ISR_CONCAT_(ID)

#define SERCOM_PTR_(ID) UART##ID
#define SERCOM_PTR(ID) SERCOM_PTR_(ID)

#define SERCOM_APBCMASK_(ID) PM_APBCMASK_SERCOM##ID
#define SERCOM_APBCMASK(ID) SERCOM_APBCMASK_(ID)

#define SERCOM_CLOCK_ID_(ID) SERCOM##ID##_GCLK_ID_CORE
#define SERCOM_CLOCK_ID(ID) SERCOM_CLOCK_ID_(ID)

#define SERCOM_IRQn_(ID) SERCOM##ID##_IRQn
#define SERCOM_IRQn(ID) SERCOM_IRQn_(ID)

unsigned int baudDef[] = {9600, 115200};

#define SYS_GPB_MFPH_PB12MFP_UART0_RXD   (0x06UL<<SYS_GPB_MFPH_PB12MFP_Pos)/*!< GPB_MFPH PB12 setting for UART0_RXD  \hideinitializer */
#define SYS_GPB_MFPH_PB13MFP_UART0_TXD   (0x06UL<<SYS_GPB_MFPH_PB13MFP_Pos)/*!< GPB_MFPH PB13 setting for UART0_TXD  \hideinitializer */

#define SYS_GPF_MFPL_PF2MFP_UART0_RXD   (0x03UL<<SYS_GPF_MFPL_PF2MFP_Pos)/*!< GPB_MFPH PB12 setting for UART0_RXD  \hideinitializer */
#define SYS_GPF_MFPL_PF3MFP_UART0_TXD   (0x03UL<<SYS_GPF_MFPL_PF3MFP_Pos)/*!< GPB_MFPH PB13 setting for UART0_TXD  \hideinitializer */

void uartInit(unsigned char uart, uint32_t baud)
{
    //uint64_t br = (uint64_t)65536 * (UP_CLK - 16 * baudDef[baud]) / UP_CLK;

    uint32_t u32UartClkSrcSel = 0ul, u32UartClkDivNum = 0ul;
    uint32_t u32ClkTbl[6ul] = {__HXT, 0ul, __LXT, __HIRC, 0ul, __LIRC};
    uint32_t u32Baud_Div = 0ul;

    switch (uart)
    {
//#ifdef UART_CHAN0
    case UART_CHAN0:

        /*Clock UART0 from HIRC*/
        CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLKSEL_Msk)) | (7 << CLK_CLKSEL0_HCLKSEL_Pos);
        CLK->CLKDIV0 = (CLK->CLKDIV0 & (~CLK_CLKDIV0_HCLKDIV_Msk)) | (0 << CLK_CLKDIV0_HCLKDIV_Pos);

        /*Enable UART0 CLK*/
        CLK->APBCLK0 |= CLK_APBCLK0_UART0CKEN_Msk;

        CLK->CLKSEL1 = (CLK->CLKSEL1 & (~CLK_CLKSEL1_UART0SEL_Msk)) | (3 << CLK_CLKSEL1_UART0SEL_Pos);
        CLK->CLKDIV0 = (CLK->CLKDIV0 & (~CLK_CLKDIV0_UART0DIV_Msk)) | (0 <<CLK_CLKDIV0_UART0DIV_Pos);

#ifdef UART_0_USE_PF2_3
        /* Set PF multi-function pins for UART0 RXD=PF.2 and TXD=PF.3 */
        SYS->GPF_MFPL = (SYS->GPF_MFPL & ~(SYS_GPF_MFPL_PF2MFP_Msk | SYS_GPF_MFPL_PF3MFP_Msk)) |
                        (SYS_GPF_MFPL_PF2MFP_UART0_RXD | SYS_GPF_MFPL_PF3MFP_UART0_TXD);
#else
        /* Set PB multi-function pins for UART0 RXD=PB.12 and TXD=PB.13 */
        SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD)) |
                        (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
#endif



        /* Select UART function */
        SERCOM_PTR(UART_CHAN0_SERCOM)->FUNCSEL = UART_FUNCSEL_UART;

        /* Set UART line configuration */
        SERCOM_PTR(UART_CHAN0_SERCOM)->LINE = UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1;

        /* Set UART Rx and RTS trigger level to 1 byte*/
        SERCOM_PTR(UART_CHAN0_SERCOM)->FIFO &= ~(UART_FIFO_RFITL_Msk | UART_FIFO_RTSTRGLV_Msk);

        UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_MODE2_DIVIDER(__HIRC, baudDef[baud]);

        /*Enable RX interrupt*/
        NVIC_EnableIRQ(UART02_IRQn);
        SERCOM_PTR(UART_CHAN0_SERCOM)->INTEN = UART_INTEN_RDAIEN_Msk;

        break;
//#endif
#ifdef UART_CHAN1
    case UART_CHAN1:
        PORT->Group[UART_CHAN1_PORT].DIRSET.reg |= (1 << UART_CHAN1_TX_PIN);  /*Tx as output*/
        PORT->Group[UART_CHAN1_PORT].DIRSET.reg &= ~(1 << UART_CHAN1_RX_PIN); /*Rx as input */

        PORT->Group[UART_CHAN1_PORT].PMUX[UART_CHAN1_TX_PIN >> 1].reg |= (UART_CHAN1_PERHIPH_TX_MUX << SAM_GPIO_MUXE);
        PORT->Group[UART_CHAN1_PORT].PMUX[UART_CHAN1_RX_PIN >> 1].reg |= (UART_CHAN1_PERHIPH_RX_MUX << SAM_GPIO_MUXO);

        PORT->Group[UART_CHAN1_PORT].PINCFG[UART_CHAN1_TX_PIN].reg |= (1 << 0); /*PMUX EN */
        PORT->Group[UART_CHAN1_PORT].PINCFG[UART_CHAN1_RX_PIN].reg |= (1 << 0); /*PMUX EN */

        PM->APBCMASK.reg |= SERCOM_APBCMASK(UART_CHAN1_SERCOM);

        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM_CLOCK_ID(UART_CHAN1_SERCOM)) |
                            GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(PERHIP_CLK_GEN);

        SERCOM_PTR(UART_CHAN1_SERCOM)->USART.CTRLA.reg =
            SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_MODE_USART_INT_CLK |
            SERCOM_USART_CTRLA_RXPO(UART_CHAN1_RX_PAD) | SERCOM_USART_CTRLA_TXPO(UART_CHAN1_TX_PAD);

        SERCOM_PTR(UART_CHAN1_SERCOM)->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN |
                SERCOM_USART_CTRLB_CHSIZE(0 /*8 bits*/);

        SERCOM_PTR(UART_CHAN1_SERCOM)->USART.BAUD.reg = (uint16_t)br;

        SERCOM_PTR(UART_CHAN1_SERCOM)->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
        SERCOM_PTR(UART_CHAN1_SERCOM)->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC; /*Enable RX interrupt*/

        NVIC_EnableIRQ(SERCOM_IRQn(UART_CHAN1_SERCOM)); /*Enable SERCOM interrupt*/
        break;
#endif
#ifdef UART_CHAN2
    case UART_CHAN2:
        PORT->Group[UART_CHAN2_PORT].DIRSET.reg |= (1 << UART_CHAN2_TX_PIN);  /*Tx as output*/
        PORT->Group[UART_CHAN2_PORT].DIRSET.reg &= ~(1 << UART_CHAN2_RX_PIN); /*Rx as input */

        PORT->Group[UART_CHAN2_PORT].PMUX[UART_CHAN2_TX_PIN >> 1].reg |= (UART_CHAN2_PERHIPH_TX_MUX << SAM_GPIO_MUXE);
        PORT->Group[UART_CHAN2_PORT].PMUX[UART_CHAN2_RX_PIN >> 1].reg |= (UART_CHAN2_PERHIPH_RX_MUX << SAM_GPIO_MUXO);

        PORT->Group[UART_CHAN2_PORT].PINCFG[UART_CHAN2_TX_PIN].reg |= (1 << 0); /*PMUX EN */
        PORT->Group[UART_CHAN2_PORT].PINCFG[UART_CHAN2_RX_PIN].reg |= (1 << 0); /*PMUX EN */

        PM->APBCMASK.reg |= SERCOM_APBCMASK(UART_CHAN2_SERCOM);

        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM_CLOCK_ID(UART_CHAN2_SERCOM)) |
                            GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(PERHIP_CLK_GEN);

        SERCOM_PTR(UART_CHAN2_SERCOM)->USART.CTRLA.reg =
            SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_MODE_USART_INT_CLK |
            SERCOM_USART_CTRLA_RXPO(UART_CHAN2_RX_PAD) | SERCOM_USART_CTRLA_TXPO(UART_CHAN2_TX_PAD);

        SERCOM_PTR(UART_CHAN2_SERCOM)->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN |
                SERCOM_USART_CTRLB_CHSIZE(0 /*8 bits*/);

        SERCOM_PTR(UART_CHAN2_SERCOM)->USART.BAUD.reg = (uint16_t)br;

        SERCOM_PTR(UART_CHAN2_SERCOM)->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
        SERCOM_PTR(UART_CHAN2_SERCOM)->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC; /*Enable RX interrupt*/

        NVIC_EnableIRQ(SERCOM_IRQn(UART_CHAN2_SERCOM)); /*Enable SERCOM interrupt*/
        break;
#endif
    }
}

void uartTx(unsigned char uart, unsigned char data)
{
    switch (uart)
    {
#ifdef UART_CHAN0
    case UART_CHAN0:
        while (!(SERCOM_PTR(UART_CHAN0_SERCOM)->FIFOSTS & UART_FIFOSTS_TXEMPTY_Msk))
            ;
        SERCOM_PTR(UART_CHAN0_SERCOM)->DAT = data;
        break;
#endif
#ifdef UART_CHAN1
    case UART_CHAN1:
        while (!(SERCOM_PTR(UART_CHAN1_SERCOM)->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_DRE))
            ;
        SERCOM_PTR(UART_CHAN1_SERCOM)->USART.DATA.reg = data;
        break;
#endif
#ifdef UART_CHAN2
    case UART_CHAN2:
        while (!(SERCOM_PTR(UART_CHAN2_SERCOM)->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_DRE))
            ;
        SERCOM_PTR(UART_CHAN2_SERCOM)->USART.DATA.reg = data;
        break;
#endif
    }
}


/* Handle int for UART0 or UART 2*/
void UART02_IRQHandler(void)
{
    /*Check for UART0 RX int*/
    if(UART0->INTSTS & UART_INTSTS_RDAINT_Msk)
    {
        uartChan0Fifo[uartChan0FifoWrite] = UART0->DAT & 0xff;
        if (uartChan0FifoWrite < UART_CHAN0_FIFO_LEN)
            uartChan0FifoWrite++;
        else
            uartChan0FifoWrite = 0;
    }
}
