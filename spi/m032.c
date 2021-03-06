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
* File: m032.c
* Description: Nuvoton M032 SPI device driver
*/

#include "printf.h"

static uint8_t rxData;

void spiInit(unsigned char channel)
{
    printf("spiInit()...");
    if(SPI_CHAN0 != channel)
    {
        printf("Incorrect channel selected!");
        return;
    }

    /*Enable UART0 CLK*/
    CLK->APBCLK0 |= CLK_APBCLK0_SPI0CKEN_Msk;

    /*Clock UART0 from HIRC*/
    CLK->CLKSEL1 |= CLK_CLKSEL2_SPI0_SRC_HIRC << CLK_CLKSEL1_UART0SEL_Pos;


    /* Set PB multi-function pins for UART0 RXD=PB.12 and TXD=PB.13 */
    // SYS->GPA_MFPL = (SYS->GPA_MFPL & ~(SYS_GPA_MFPL_PA0MFP_Msk | SYS_GPA_MFPL_PA1MFP_Msk)) |
    //                 (SYS_GPA_MFPL_PA0MFP_SPI0_MOSI | SYS_GPA_MFPL_PA1MFP_SPI0_MISO);


    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~(SYS_GPA_MFPL_PA3MFP_Msk |
                                       SYS_GPA_MFPL_PA2MFP_Msk |
                                       SYS_GPA_MFPL_PA1MFP_Msk |
                                       SYS_GPA_MFPL_PA0MFP_Msk)) |
                    (SYS_GPA_MFPL_PA3MFP_SPI0_SS |
                     SYS_GPA_MFPL_PA2MFP_SPI0_CLK |
                     SYS_GPA_MFPL_PA1MFP_SPI0_MISO |
                     SYS_GPA_MFPL_PA0MFP_SPI0_MOSI);

    /* Disable I2S mode */
    SPI0->I2SCTL &= ~SPI_I2SCTL_I2SEN_Msk;

    /*Setup clock div to default to 100kHz*/
    SPI0->CLKDIV = (UP_CLK / 100000)+1;

    /* Default setting: slave selection signal is active low; disable automatic slave selection function. */
    // SPI0->SSCTL = SPI_SS;

    /* Default setting: MSB first, Master and 8 data bits */
    SPI0->CTL = (8 << SPI_CTL_DWIDTH_Pos) | SPI_CTL_SPIEN_Msk;


    printf("Done.\r\n");
}

void spiTxByte(unsigned char channel, unsigned char byte)
{
    if(SPI_CHAN0 != channel)
    {
        printf("spiTxByte: Incorrect channel selected!");
        return;
    }
    SPI0->TX = byte;

    while(!SPI0->STATUS & SPI_STATUS_UNITIF_Msk);
    rxData = SPI0->RX;
}

unsigned char spiRxByte(unsigned char channel)
{
    if(SPI_CHAN0 != channel)
    {
        printf("spiRxByte: Incorrect channel selected!");
        return;
    }
    return rxData;
}