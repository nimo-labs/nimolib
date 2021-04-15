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
* File: atsamd21.c
* Description: ATSAMD21 SPI device driver
*/

/*Required defines in nimolib.h
*
* I2C_CHAN0
* I2C_CHAN0_PORT
* I2C_CHAN0_SDA_PIN
* I2C_CHAN0_SCL_PIN
* I2C_CHAN0_PERHIPH_TX_MUX
* I2C_CHAN0_PERHIPH_RX_MUX
* I2C_CHAN0_SERCOM
*
*/

#include <delay.h>
#include <printf.h>

#define ISR_CONCAT_(ID) SERCOM##ID##_Handler()
#define ISR_CONCAT(ID) ISR_CONCAT_(ID)

#define SERCOM_PTR_(ID) SERCOM##ID
#define SERCOM_PTR(ID) SERCOM_PTR_(ID)

#define SERCOM_APBCMASK_(ID) PM_APBCMASK_SERCOM##ID
#define SERCOM_APBCMASK(ID) SERCOM_APBCMASK_(ID)

#define SERCOM_CLOCK_ID_(ID) SERCOM##ID##_GCLK_ID_CORE
#define SERCOM_CLOCK_ID(ID) SERCOM_CLOCK_ID_(ID)

#define SERCOM_IRQn_(ID) SERCOM##ID##_IRQn
#define SERCOM_IRQn(ID) SERCOM_IRQn_(ID)

//static unsigned int baud = 0;

#define I2C_TRANSFER_WRITE 0
#define I2C_TRANSFER_READ 1

void i2cInit(unsigned char channel, unsigned int baudRate)
{
    printf("i2cInit()...");

    int32_t tmp_baud;
    if(I2C_BAUD_100K == baudRate)
        tmp_baud = (int32_t)(((UP_CLK + (2*(100000)) - 1) / (2*(100000))) - 5);
    else if(I2C_BAUD_400K == baudRate)
        tmp_baud = (int32_t)(((UP_CLK + (2*(400000)) - 1) / (2*(400000))) - 5);

    switch(channel)
    {
#ifdef I2C_CHAN0
    case I2C_CHAN0:
        PORT->Group[I2C_CHAN0_PORT].DIRSET.reg |= (1 << I2C_CHAN0_SDA_PIN);  /*Tx as output*/
        PORT->Group[I2C_CHAN0_PORT].DIRSET.reg &= ~(1 << I2C_CHAN0_SCL_PIN); /*Rx as input */

        PORT->Group[I2C_CHAN0_PORT].PMUX[I2C_CHAN0_SDA_PIN >> 1].reg |= (I2C_CHAN0_PERHIPH_TX_MUX << SAM_GPIO_MUXE);
        PORT->Group[I2C_CHAN0_PORT].PMUX[I2C_CHAN0_SCL_PIN >> 1].reg |= (I2C_CHAN0_PERHIPH_RX_MUX << SAM_GPIO_MUXO);

        PORT->Group[I2C_CHAN0_PORT].PINCFG[I2C_CHAN0_SDA_PIN].reg |= (1 << 0); /*PMUX EN */
        PORT->Group[I2C_CHAN0_PORT].PINCFG[I2C_CHAN0_SCL_PIN].reg |= (1 << 0); /*PMUX EN */

        PM->APBCMASK.reg |= SERCOM_APBCMASK(I2C_CHAN0_SERCOM);

        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM_CLOCK_ID(I2C_CHAN0_SERCOM)) |
                            GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

        SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLB.reg = SERCOM_I2CM_CTRLB_SMEN;
        while (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.SYNCBUSY.reg)
            ;

        if (tmp_baud < 255 && tmp_baud > 0)
        {
            SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLA.reg |= SERCOM_I2CM_CTRLA_SPEED(0);
            SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.BAUD.reg = SERCOM_I2CM_BAUD_BAUD(tmp_baud);
        }
        while (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.SYNCBUSY.reg)
            ;

        SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLA.reg = SERCOM_I2CM_CTRLA_ENABLE |
                SERCOM_I2CM_CTRLA_MODE_I2C_MASTER |
                SERCOM_I2CM_CTRLA_SDAHOLD(3);
        while (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.SYNCBUSY.reg)
            ;

        SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.STATUS.reg |= SERCOM_I2CM_STATUS_BUSSTATE(1);
        while (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.SYNCBUSY.reg)
            ;
        break;
#endif
#ifdef I2C_CHAN1
    case I2C_CHAN1:
        PORT->Group[I2C_CHAN1_PORT].DIRSET.reg |= (1 << I2C_CHAN1_SDA_PIN);  /*Tx as output*/
        PORT->Group[I2C_CHAN1_PORT].DIRSET.reg &= ~(1 << I2C_CHAN1_SCL_PIN); /*Rx as input */

        PORT->Group[I2C_CHAN1_PORT].PMUX[I2C_CHAN1_SDA_PIN >> 1].reg |= (I2C_CHAN1_PERHIPH_TX_MUX << SAM_GPIO_MUXE);
        PORT->Group[I2C_CHAN1_PORT].PMUX[I2C_CHAN1_SCL_PIN >> 1].reg |= (I2C_CHAN1_PERHIPH_RX_MUX << SAM_GPIO_MUXO);

        PORT->Group[I2C_CHAN1_PORT].PINCFG[I2C_CHAN1_SDA_PIN].reg |= (1 << 0); /*PMUX EN */
        PORT->Group[I2C_CHAN1_PORT].PINCFG[I2C_CHAN1_SCL_PIN].reg |= (1 << 0); /*PMUX EN */

        PM->APBCMASK.reg |= SERCOM_APBCMASK(I2C_CHAN1_SERCOM);

        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM_CLOCK_ID(I2C_CHAN1_SERCOM)) |
                            GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

        SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg = SERCOM_I2CM_CTRLB_SMEN;
        while (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.SYNCBUSY.reg)
            ;

        if (tmp_baud < 255 && tmp_baud > 0)
        {
            SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLA.reg |= SERCOM_I2CM_CTRLA_SPEED(0);
            SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.BAUD.reg = SERCOM_I2CM_BAUD_BAUD(tmp_baud);
        }
        while (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.SYNCBUSY.reg)
            ;

        SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLA.reg = SERCOM_I2CM_CTRLA_ENABLE |
                SERCOM_I2CM_CTRLA_MODE_I2C_MASTER |
                SERCOM_I2CM_CTRLA_SDAHOLD(3);
        while (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.SYNCBUSY.reg)
            ;

        SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.STATUS.reg |= SERCOM_I2CM_STATUS_BUSSTATE(1);
        while (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.SYNCBUSY.reg)
            ;
        break;
#endif
    }

    printf("Done.\r\n");
}

// static void i2cStart(unsigned char address)
// {
//     unsigned long delayLast;
//     SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.ADDR.reg = address | I2C_TRANSFER_WRITE;
//     delayLast = delayGetTicks();

//     while (0 == (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB))
//     {
//         if (delayMillis(delayLast, 1000))
//         {
//             SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
//             printf("I2C timeout\r\n");
//             return;
//         }
//     }

//     if (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_RXNACK)
//     {
//         SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
//     }
// }

static void i2cStop(unsigned char channel)
{
    switch(channel)
    {
#ifdef I2C_CHAN0
    case I2C_CHAN0:
        SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
        break;
#endif
#ifdef I2C_CHAN1
    case I2C_CHAN1:
        SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
        break;
#endif
    }
}

unsigned char i2cWrite(unsigned char channel, unsigned char address, unsigned char *data, unsigned int len, unsigned char stop)
{
    unsigned int i;
    unsigned long delayLast;


    switch(channel)
    {
#ifdef I2C_CHAN0
    case I2C_CHAN0:
        SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.ADDR.reg = (address << 1) | I2C_TRANSFER_WRITE;

        delayLast = delayGetTicks();
        while (0 == (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB))
        {
            if (delayMillis(delayLast, 1000))
            {
                SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
                printf("I2C timeout\r\n");
                return TRUE;
            }
        }
        //    while (0 == (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB));
        if (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_RXNACK)
        {
            SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
            //    printf("I2C: RXNACK during write (address)\r\n");
            return TRUE;
        }

        for (i = 0; i < len; i++)
        {
            /*Write value*/
            SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.DATA.reg = data[i];

            //   while (0 == (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB));
            delayLast = delayGetTicks();
            while (0 == (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB))
            {
                if (delayMillis(delayLast, 1000))
                {
                    SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
                    printf("I2C timeout\r\n");
                    return TRUE;
                }
            }

            if (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_RXNACK)
            {
                SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
                printf("I2C: RXNACK during write (data)\r\n");
                return TRUE;
            }
        }
        if (stop > 0)
            i2cStop(I2C_CHAN0);
        return FALSE;
        break;
#endif

#ifdef I2C_CHAN1
    case I2C_CHAN1:
        SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.ADDR.reg = (address << 1) | I2C_TRANSFER_WRITE;

        delayLast = delayGetTicks();
        while (0 == (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB))
        {
            if (delayMillis(delayLast, 1000))
            {
                SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
                printf("I2C timeout\r\n");
                return TRUE;
            }
        }
        //    while (0 == (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB));
        if (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_RXNACK)
        {
            SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
            //    printf("I2C: RXNACK during write (address)\r\n");
            return TRUE;
        }

        for (i = 0; i < len; i++)
        {
            /*Write value*/
            SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.DATA.reg = data[i];

            //   while (0 == (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB));
            delayLast = delayGetTicks();
            while (0 == (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_MB))
            {
                if (delayMillis(delayLast, 1000))
                {
                    SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
                    printf("I2C timeout\r\n");
                    return TRUE;
                }
            }

            if (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_RXNACK)
            {
                SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
                printf("I2C: RXNACK during write (data)\r\n");
                return TRUE;
            }
        }
        if (stop > 0)
            i2cStop(I2C_CHAN1);
        return FALSE;
        break;
#endif
    }
}

unsigned char i2cRead(unsigned char channel, unsigned char address, unsigned char *data, unsigned int len)
{
    unsigned int i;
    unsigned long delayLast;

    switch(channel)
    {
#ifdef I2C_CHAN0
    case I2C_CHAN0:
        /*Read*/
        SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.ADDR.reg = (address << 1) | I2C_TRANSFER_READ;

        //    while (0 == (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_SB));

        delayLast = delayGetTicks();
        while (0 == (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_SB))
        {
            if (delayMillis(delayLast, 4000))
            {
                SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
                printf("I2C read timeout\r\n");
                return TRUE;
            }
        }

        if (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_RXNACK)
        {
            SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
            printf("I2C: RXNACK during read (address)\r\n");
            return TRUE;
        }

        SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;

        for (i = 0; i < len - 1; i++)
        {
            data[i] = SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.DATA.reg;
            while (0 == (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_SB))
                ;
            delayLast = delayGetTicks();
            while (0 == (SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_SB))
            {
                if (delayMillis(delayLast, 4000))
                {
                    SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
                    printf("I2C read timeout\r\n");
                    return TRUE;
                }
            }
        }

        if (len)
        {
            SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT;
            SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
            data[len - 1] = SERCOM_PTR(I2C_CHAN0_SERCOM)->I2CM.DATA.reg;
        }

        return FALSE;
        break;
#endif
#ifdef I2C_CHAN1
    case I2C_CHAN1:
        /*Read*/
        SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.ADDR.reg = (address << 1) | I2C_TRANSFER_READ;

        //    while (0 == (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_SB));

        delayLast = delayGetTicks();
        while (0 == (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_SB))
        {
            if (delayMillis(delayLast, 4000))
            {
                SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
                printf("I2C read timeout\r\n");
                return TRUE;
            }
        }

        if (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_RXNACK)
        {
            SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
            printf("I2C: RXNACK during read (address)\r\n");
            return TRUE;
        }

        SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;

        for (i = 0; i < len - 1; i++)
        {
            data[i] = SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.DATA.reg;
            while (0 == (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_SB))
                ;
            delayLast = delayGetTicks();
            while (0 == (SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.INTFLAG.reg & SERCOM_I2CM_INTFLAG_SB))
            {
                if (delayMillis(delayLast, 4000))
                {
                    SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
                    printf("I2C read timeout\r\n");
                    return TRUE;
                }
            }
        }

        if (len)
        {
            SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT;
            SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
            data[len - 1] = SERCOM_PTR(I2C_CHAN1_SERCOM)->I2CM.DATA.reg;
        }

        return FALSE;
        break;
#endif
    }
}
