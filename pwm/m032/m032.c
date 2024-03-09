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
#include <pwm.h>

#include "m032.h"

void setupPwmChan(uint32_t u32ChannelNum)
{
    /* Set BPWM Timer clock prescaler */
    BPWM_SET_PRESCALER(BPWM0, u32ChannelNum, 0); /* Divided by 1 */

    /* Set BPWM Timer duty */
    BPWM_SET_CMR(BPWM0, u32ChannelNum, 19200);

    /* Set BPWM Timer period */
    BPWM_SET_CNR(BPWM0, u32ChannelNum, 38399);

    /* Set waveform generation */
    BPWM_SET_OUTPUT_LEVEL(BPWM0, BPWM_CH_0_MASK, BPWM_OUTPUT_HIGH, BPWM_OUTPUT_LOW, BPWM_OUTPUT_NOTHING, BPWM_OUTPUT_NOTHING);
}

void pwmInit(void)
{
    /* Enable BPWM0 module clock */
    CLK->APBCLK1 |= CLK_APBCLK1_BPWM0CKEN_Msk;

    /* BPWM clock frequency can be set equal or double to HCLK by choosing case 1 or case 2 */
    CLK->CLKSEL2 = (CLK->CLKSEL2 & ~CLK_CLKSEL2_BPWM0SEL_Msk) | CLK_CLKSEL2_BPWM0SEL_PCLK0;

    /* Reset BPWM0 module */
    SYS->IPRST2 |= SYS_IPRST2_BPWM0RST_Msk;
    SYS->IPRST2 &= ~SYS_IPRST2_BPWM0RST_Msk;

    /*
      Configure BPWM0 channel 0 init period and duty(up counter type).
      Period is PLL / (prescaler * (CNR + 1))
      Duty ratio = (CMR) / (CNR + 1)
      Period = 48 MHz / (1 * (38399 + 1)) = 1250 Hz
      Duty ratio = (19200) / (38399 + 1) = 50%
    */

    /* Set BPWM to up counter type(edge aligned) */
    BPWM0->CTL1 &= ~BPWM_CTL1_CNTTYPE0_Msk;

    /* Enable output of BPWM0 channel 0 */
    BPWM0->POEN |= BPWM_POEN_POEN0_Msk;

    /* Start */
    BPWM0->CNTEN |= BPWM_CNTEN_CNTEN0_Msk;

}

/**
 * @brief       Calculate the comparator value of new duty by configured period
 *
 * @param       bpwm                  The pointer of the specified BPWM module
 *
 * @param       u32ChannelNum        BPWM channel number. Valid values are between 0~5
 *
 * @param       u32DutyCycle         Target generator duty cycle percentage. Valid range are between 0 ~ u32CycleResolution.
 *                                   If u32CycleResolution is 100, and u32DutyCycle is 10 means 10%, 20 means 20% ...
 *
 * @param       u32CycleResolution   Target generator duty cycle resolution. The value in general is 100.
 *
 * @return      The compatator value by new duty cycle
 */
static uint32_t CalNewDutyCMR(BPWM_T *bpwm, uint32_t u32ChannelNum, uint32_t u32DutyCycle, uint32_t u32CycleResolution)
{
    if (u32DutyCycle >= u32CycleResolution)
        return BPWM_GET_CNR(bpwm, u32ChannelNum);

    return (u32DutyCycle * (BPWM_GET_CNR(bpwm, u32ChannelNum) + 1) / u32CycleResolution);
}

void pwmSetChanDuty(uint8_t channel, uint32_t dutyCycle)
{
    uint32_t u32NewCMR = 0;
    /* Get new comparator value by call CalNewDutyCMR() */
    u32NewCMR = CalNewDutyCMR(BPWM0, channel, dutyCycle, 100);
    /* Set new comparator value to register */
    BPWM_SET_CMR(BPWM0, channel, u32NewCMR);
}