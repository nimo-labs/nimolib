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
    UI2C_T * ui2c = UI2C0;
    UI2C_STOP(ui2c);
    while (!(UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U));
    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STORIF_Msk);      /* Clear Stop INT Flag */
}



unsigned char i2cWrite(unsigned char channel, unsigned char address, unsigned char *data, unsigned int len, unsigned char stop)
{
    UI2C_T * ui2c = UI2C0;

    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_ACKIF_Msk);
    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_NACKIF_Msk);
    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STORIF_Msk);

    /*Send Start */
    UI2C_SET_CONTROL_REG(ui2c,(UI2C_CTL_PTRG | UI2C_CTL_STA));
    while (!(UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U));
    if(UI2C_PROTSTS_STARIF_Msk != UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U)
        return 1;
    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STARIF_Msk);

    /*Send Chip address */
    UI2C_SET_DATA(ui2c, (address << 1U) | 0x00U);
    UI2C_SET_CONTROL_REG(ui2c,UI2C_CTL_PTRG);
    while (!(UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U));

    if(UI2C_PROTSTS_ACKIF_Msk == (UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U))
    {
        UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_ACKIF_Msk);      /* Clear ACK INT Flag */
        UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STORIF_Msk);
        //return 1;
    }

    if(UI2C_PROTSTS_NACKIF_Msk == (UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U))
    {
        UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_NACKIF_Msk);      /* Clear NACK INT Flag */
        UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STORIF_Msk);
        i2cStop(channel);
        return 1;
    }

    /*Send Data */
    for(uint32_t i=0; i < len; i++)
    {
        UI2C_SET_DATA(ui2c, data[i]);
        UI2C_SET_CONTROL_REG(ui2c,UI2C_CTL_PTRG);
        while (!(UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U));
        if(UI2C_PROTSTS_ACKIF_Msk != (UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U))
            return 1;
        UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_ACKIF_Msk);      /* Clear ACK INT Flag */
    }

    /*Send Stop */
    if(stop)
    {
        i2cStop(channel);
    }

    /*Cleanup */
    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STORIF_Msk);
    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_NACKIF_Msk);
    return 0;
}

unsigned char i2cRead(unsigned char channel, unsigned char address, unsigned char *data, unsigned int len)
{
    UI2C_T * ui2c = UI2C0;

    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_ACKIF_Msk);
    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_NACKIF_Msk);
    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STORIF_Msk);
    /*Send Re Start */
    UI2C_SET_CONTROL_REG(ui2c,(UI2C_CTL_PTRG | UI2C_CTL_STA));
    while (!(UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U));

    if(UI2C_PROTSTS_STARIF_Msk != (UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U))
    {
        i2cStop(channel);
        return 1;
    }
    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STARIF_Msk);

    /*Send Chip address */
    UI2C_SET_DATA(ui2c, (address << 1U) | 0x01U);
    UI2C_SET_CONTROL_REG(ui2c,UI2C_CTL_PTRG);
    while (!(UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U));
    if(UI2C_PROTSTS_ACKIF_Msk != (UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U))
    {
        i2cStop(channel);
        UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_NACKIF_Msk);      /* Clear NACK INT Flag */
        return 1;
    }
    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_ACKIF_Msk);      /* Clear ACK INT Flag */

    /*Get Data */
    for(uint32_t i=0; i < len; i++)
    {
        if(i+1 < len)
            UI2C_SET_CONTROL_REG(ui2c,UI2C_CTL_PTRG| UI2C_CTL_AA);
        else
            UI2C_SET_CONTROL_REG(ui2c,UI2C_CTL_PTRG);
        while (!(UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U));
        data[i] = (uint8_t) UI2C_GET_DATA(ui2c);

        while (!(UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U));
        UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_ACKIF_Msk);      /* Clear ACK INT Flag */
    }

    /*Send stop */
    UI2C_SET_CONTROL_REG(ui2c,(UI2C_CTL_PTRG | UI2C_CTL_STO));
    while (!(UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U));
    /*Cleanup */

    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_STORIF_Msk);
    UI2C_CLR_PROT_INT_FLAG(ui2c, UI2C_PROTSTS_NACKIF_Msk);
    while (!(UI2C_GET_PROT_STATUS(ui2c) & 0x3F00U));
    return 0;
}