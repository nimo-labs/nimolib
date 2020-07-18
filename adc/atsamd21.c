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
* Description: SAMD21 ADC device driver
*/

#include <sam.h>
#include <nimolib.h>
#include <printf.h>

#include "adc.h"

unsigned char adcOffset = 190; /*ADC single ended offset*/

/** \brief returns the value of the requested ADC channel
*
* This function averages the requested channel over 8 samples and returns
* the averaged result.
*/
unsigned int adcReadChan(unsigned char channel) // Mode = 1 for single ended, 0 for internal
{
    long result = 0;
    unsigned int ctr;

    result += adcReadChanSingle(channel);
    result = 0;
    for (ctr = 0; ctr < 8; ctr++)
        result += adcReadChanSingle(channel);

    result /= 8;

    return (unsigned int)result;
}

/** \brief returns the value of the requested ADC channel
 *
 * This function returns a single ADC result with no averaging.
*/
unsigned int adcReadChanSingle(unsigned char channel) // Mode = 1 for single ended, 0 for internal
{
    long result = 0;

    //ADC->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN_1X | ADC_INPUTCTRL_MUXNEG_IOGND | ADC_INPUTCTRL_MUXPOS_PIN17; /*Force input to int therm for now*/
    ADC->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN_1X | ADC_INPUTCTRL_MUXNEG_IOGND | ADC_INPUTCTRL_MUXPOS(channel); /*Force input to PV_V for now*/

    ADC->SWTRIG.reg |= ADC_SWTRIG_START; /*Start conversion*/

    while (!(ADC->INTFLAG.reg & ADC_INTFLAG_RESRDY))
        ;

    result = ADC->RESULT.reg;

    return (unsigned int)result;
}

/** \brief ADC initialisation routine
 *
 * Currently this function initialises the ADC module along with channel 0.
 * It also calls adcCalcOffset() to calculate the actual offset.
 *
 *
 */
void adcInit(void)
{
    printf("adcInit()...");
    /*Setup clock first!*/
    PM->APBCMASK.reg |= PM_APBCMASK_ADC;

    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(ADC_GCLK_ID) |
                        GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(PERHIP_CLK_GEN);
    ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_INT1V; /*Use internal 1V reference*/

    ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV64 | ADC_CTRLB_RESSEL_12BIT;

    ADC->CTRLA.reg = ADC_CTRLA_ENABLE; /*Enable ADC*/
    printf("Done.\r\n");
}
