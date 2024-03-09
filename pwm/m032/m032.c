/*
* Copyright 2024 Nicholas Dark.
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
* File: PWM.c
* Description: Nuvoton M032 PWM driver
*/

#include "M031Series.h"
#include <printf.h>
#include <adc.h>

#include "pwm.h"

void adcInit(void)
{
    /* Enable BPWM0 module clock */
    CLK->APBCLK1 |= CLK_APBCLK1_BPWM0CKEN_Msk;

    /* BPWM clock frequency can be set equal or double to HCLK by choosing case 1 or case 2 */
    CLK->CLKSEL2 = (CLK->CLKSEL2 & ~CLK_CLKSEL2_BPWM0SEL_Msk) | CLK_CLKSEL2_BPWM0SEL_PCLK0;

    /* Reset BPWM0 module */
    SYS->IPRST2 |= SYS_IPRST2_BPWM0RST_Msk;
    SYS->IPRST2 &= ~SYS_IPRST2_BPWM0RST_Msk;

    /* Set PA multi-function pin for BPWM0 Channel 0 */
    SYS->GPA_MFPH = (SYS->GPA_MFPL & (~SYS_GPA_MFPH_PA11MFP_Msk)) | SYS_GPA_MFPH_PA11MFP_BPWM0_CH0;

    /*
      Configure BPWM0 channel 0 init period and duty(up counter type).
      Period is PLL / (prescaler * (CNR + 1))
      Duty ratio = (CMR) / (CNR + 1)
      Period = 48 MHz / (1 * (38399 + 1)) = 1250 Hz
      Duty ratio = (19200) / (38399 + 1) = 50%
    */

    /* Set BPWM to up counter type(edge aligned) */
    BPWM0->CTL1 &= ~BPWM_CTL1_CNTTYPE0_Msk;

    /* Set BPWM Timer clock prescaler */
    BPWM_SET_PRESCALER(BPWM0, 0, 0); /* Divided by 1 */

    /* Set BPWM Timer duty */
    BPWM_SET_CMR(BPWM0, 0, 19200);

    /* Set BPWM Timer period */
    BPWM_SET_CNR(BPWM0, 0, 38399);

    /* Set waveform generation */
    BPWM_SET_OUTPUT_LEVEL(BPWM0, BPWM_CH_0_MASK, BPWM_OUTPUT_HIGH, BPWM_OUTPUT_LOW, BPWM_OUTPUT_NOTHING, BPWM_OUTPUT_NOTHING);

    /* Enable output of BPWM0 channel 0 */
    BPWM0->POEN |= BPWM_POEN_POEN0_Msk;

    /* Start */
    BPWM0->CNTEN |= BPWM_CNTEN_CNTEN0_Msk;

}

