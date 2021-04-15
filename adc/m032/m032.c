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
* File: adc.c
* Description: Nuvoton M032 ADC driver
*/

#include "M031Series.h"
#include <printf.h>
#include <adc.h>

#include "m032.h"

void adcInit(void)
{
    CLK->APBCLK0 |= CLK_APBCLK0_ADCCKEN_Msk;
    //CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV2);
    //CLK->CLKSEL2 = (CLK->CLKSEL2 & ~CLK_CLKSEL2_ADCSEL_Msk) | CLK_CLKSEL2_ADC_SRC_PCLK1;
    CLK->CLKDIV0 = (CLK->CLKDIV0 & ~CLK_CLKDIV0_ADCDIV_Msk) | CLK_CLKDIV0_ADC(20);
    /* Enable ADC converter */
    ADC->ADCR |= ADC_ADCR_ADEN_Msk;

    /* Do calibration for ADC to decrease the effect of electrical random noise. */
    ADC->ADCALSTSR |= ADC_ADCALSTSR_CALIF_Msk;  /* Clear Calibration Finish Interrupt Flag */
    ADC->ADCALR |= ADC_ADCALR_CALEN_Msk;        /* Enable Calibration function */
    ADC_START_CONV(ADC);                        /* Start to calibration */
    while((ADC->ADCALSTSR & ADC_ADCALSTSR_CALIF_Msk) != ADC_ADCALSTSR_CALIF_Msk);   /* Wait calibration finish */
}

unsigned int adcReadChan(unsigned char channel)
{

}

unsigned int adcReadChanSingle(unsigned char channel)
{
    uint32_t u32ConversionData;
    /* Set input mode as single-end, Single mode, and select channel */
    ADC->ADCR = (ADC->ADCR & ~(ADC_ADCR_DIFFEN_Msk | ADC_ADCR_ADMD_Msk)) |
                (ADC_ADCR_DIFFEN_SINGLE_END | ADC_ADCR_ADMD_SINGLE | ADC_ADCR_ADIE_Msk);
    ADC->ADCHER = 0;
    ADC->ADCHER |= (1 << channel);

    /* Clear the A/D interrupt flag for safe */
    ADC->ADSR0 = ADC_ADF_INT;

    /* Enable the sample module interrupt */
//  NVIC_EnableIRQ(ADC_IRQn);

    /* Reset the ADC interrupt indicator and trigger sample module 0 to start A/D conversion */
// g_u32AdcIntFlag = 0;
    ADC->ADCR |= ADC_ADCR_ADST_Msk;

    /* Wait ADC interrupt (g_u32AdcIntFlag will be set at IRQ_Handler function) */
//while(g_u32AdcIntFlag == 0);
    while(!(ADC->ADSR0 & ADC_ADSR0_ADF_Msk));
    ADC->ADSR0 |= ADC_ADSR0_ADF_Msk;
    /* Disable the sample module interrupt */
// ADC->ADCR &= ~ADC_ADCR_ADIE_Msk;

    /* Get the conversion result of ADC channel 2 */
    u32ConversionData = (ADC->ADDR[channel] & ADC_ADDR_RSLT_Msk);
    //printf("Conversion result of channel %d: 0x%X (%d)\r\n", channel,u32ConversionData, u32ConversionData);
    return u32ConversionData;
}