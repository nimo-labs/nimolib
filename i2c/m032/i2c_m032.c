#include "M031Series.h"


void i2cInit(unsigned char channel, unsigned int baudRate)
{
    UI2C_T * ui2c = UI2C0;
    uint32_t u32BusClock = 100000; /* 100kHz */
    /*Enable USCI0 CLK*/
    CLK->APBCLK1 |= CLK_APBCLK1_USCI0CKEN_Msk;

    /* Set UI2C0 multi-function pins */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk)) |
                    (SYS_GPB_MFPH_PB12MFP_USCI0_CLK | SYS_GPB_MFPH_PB13MFP_USCI0_DAT0);


    uint32_t u32ClkDiv;
    uint32_t u32Pclk;

    if (ui2c == UI2C0)
    {
        u32Pclk = UP_CLK;
    }

    u32ClkDiv = (uint32_t)((((((u32Pclk / 2U) * 10U) / (u32BusClock)) + 5U) / 10U) - 1U); /* Compute proper divider for USCI_I2C clock */

    /* Enable USCI_I2C protocol */
    ui2c->CTL &= ~UI2C_CTL_FUNMODE_Msk;
    ui2c->CTL = 4U << UI2C_CTL_FUNMODE_Pos;

    /* Data format configuration */
    /* 8 bit data length */
    ui2c->LINECTL &= ~UI2C_LINECTL_DWIDTH_Msk;
    ui2c->LINECTL |= 8U << UI2C_LINECTL_DWIDTH_Pos;

    /* MSB data format */
    ui2c->LINECTL &= ~UI2C_LINECTL_LSB_Msk;

    /* Set USCI_I2C bus clock */
    ui2c->BRGEN &= ~UI2C_BRGEN_CLKDIV_Msk;
    ui2c->BRGEN |= (u32ClkDiv << UI2C_BRGEN_CLKDIV_Pos);
    ui2c->PROTCTL |=  UI2C_PROTCTL_PROTEN_Msk;

}

static void i2cStop(unsigned char channel)
{
    // I2C0->CTL0 = (I2C0->CTL0 & 0x3C) | I2C_CTL_STO_SI;
}

unsigned char i2cWrite(unsigned char channel, unsigned char address, unsigned char *data, unsigned int len, unsigned char stop)
{
    UI2C_T * ui2c = UI2C0;
    uint8_t u8Xfering = 1U, u8Err = 0U, u8Ctrl = 0U;
    enum UI2C_MASTER_EVENT eEvent = MASTER_SEND_START;

    UI2C_START(ui2c);                                                       /* Send START */

    while (u8Xfering)
    {
        while (!(UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U));                     /* Wait UI2C new status occur */

        switch (UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U)
        {
        case UI2C_PROTSTS_STARIF_Msk:
            UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STARIF_Msk);     /* Clear START INT Flag */
            UI2C_SET_DATA(ui2c, (address << 1U) | 0x00U);             /* Write SLA+W to Register UI2C_TXDAT */
            eEvent = MASTER_SEND_ADDRESS;
            u8Ctrl = UI2C_CTL_PTRG;                                     /* Clear SI */
            break;

        case UI2C_PROTSTS_ACKIF_Msk:
            UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_ACKIF_Msk);      /* Clear ACK INT Flag */

            if (eEvent == MASTER_SEND_ADDRESS)
            {
                UI2C_SET_DATA(ui2c, data);                              /* Write data to UI2C_TXDAT */
                eEvent = MASTER_SEND_DATA;
            }
            else
            {
                u8Ctrl = (UI2C_CTL_PTRG | UI2C_CTL_STO);                /* Clear SI and send STOP */
            }

            break;

        case UI2C_PROTSTS_NACKIF_Msk:
            UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_NACKIF_Msk);     /* Clear NACK INT Flag */
            u8Ctrl = (UI2C_CTL_PTRG | UI2C_CTL_STO);                    /* Clear SI and send STOP */
            u8Err = 1U;
            break;

        case UI2C_PROTSTS_STORIF_Msk:
            UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STORIF_Msk);     /* Clear STOP INT Flag */
            u8Ctrl = UI2C_CTL_PTRG;                                     /* Clear SI */
            u8Xfering = 0U;
            break;

        case UI2C_PROTSTS_ARBLOIF_Msk:                                  /* Arbitration Lost */
        default:                                                        /* Unknow status */
            u8Ctrl = (UI2C_CTL_PTRG | UI2C_CTL_STO);                    /* Clear SI and send STOP */
            u8Err = 1U;
            break;
        }

        UI2C_SET_CONTROL_REG(ui2c, u8Ctrl);                                 /* Write controlbit to UI2C_PROTCTL register */
    }

    return (u8Err | u8Xfering);                                             /* return (Success)/(Fail) status */
}

unsigned char i2cRead(unsigned char channel, unsigned char address, unsigned char *data, unsigned int len)
{
    UI2C_T * ui2c = UI2C0;
    uint8_t u8Xfering = 1U, u8Err = 0U, rdata = 0U, u8Ctrl = 0U;
    enum UI2C_MASTER_EVENT eEvent = MASTER_SEND_START;

    UI2C_START(ui2c);                                                       /* Send START */

    while (u8Xfering)
    {
        while (!(UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U));                     /* Wait UI2C new status occur */

        switch (UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U)
        {
        case UI2C_PROTSTS_STARIF_Msk:
            UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STARIF_Msk);     /* Clear START INT Flag */
            UI2C_SET_DATA(ui2c, (address << 1U) | 0x01U);             /* Write SLA+R to Register UI2C_TXDAT */
            eEvent = MASTER_SEND_H_RD_ADDRESS;
            u8Ctrl = UI2C_CTL_PTRG;
            break;

        case UI2C_PROTSTS_ACKIF_Msk:
            UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_ACKIF_Msk);      /* Clear ACK INT Flag */
            eEvent = MASTER_READ_DATA;
            break;

        case UI2C_PROTSTS_NACKIF_Msk:
            UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_NACKIF_Msk);     /* Clear NACK INT Flag */

            if (eEvent == MASTER_SEND_H_RD_ADDRESS)
            {
                u8Err = 1U;
            }
            else
            {
                rdata = (unsigned char) UI2C_GET_DATA(ui2c);            /* Receive Data */
            }

            u8Ctrl = (UI2C_CTL_PTRG | UI2C_CTL_STO);                        /* Clear SI and send STOP */

            break;

        case UI2C_PROTSTS_STORIF_Msk:
            UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STORIF_Msk);     /* Clear STOP INT Flag */
            u8Ctrl = UI2C_CTL_PTRG;                                     /* Clear SI */
            u8Xfering = 0U;
            break;

        case UI2C_PROTSTS_ARBLOIF_Msk:                                  /* Arbitration Lost */
        default:                                                        /* Unknow status */
            u8Ctrl = (UI2C_CTL_PTRG | UI2C_CTL_STO);                    /* Clear SI and send STOP */
            u8Err = 1U;
            break;
        }

        UI2C_SET_CONTROL_REG(ui2c, u8Ctrl);                                 /* Write controlbit to UI2C_PROTCTL register */
    }

    if (u8Err)
        rdata = 0U;

    return rdata;                                                           /* Return read data */
}