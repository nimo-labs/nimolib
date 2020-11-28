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
* Description: ATSAMD21 TC device driver
*/


void tmrCtrInit(unsigned int tickInterval) /* tick in tickInterval sec*/
{
    PM->APBCMASK.reg |= PM_APBCMASK_TC4 | PM_APBCMASK_TC5;

    GCLK->CLKCTRL.reg =  GCLK_CLKCTRL_ID_TC4_TC5 |
                         GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(PERHIP_CLK_GEN);
    while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

    TC4->COUNT32.CTRLA.reg = 0;
    TC4->COUNT32.CTRLA.reg |= TC_CTRLA_PRESCSYNC_GCLK | TC_CTRLA_RUNSTDBY | TC_CTRLA_PRESCALER_DIV1024 |
                              TC_CTRLA_WAVEGEN_MFRQ | TC_CTRLA_MODE_COUNT32 | TC_CTRLA_ENABLE;
    while (TC4->COUNT32.STATUS.bit.SYNCBUSY);        // Wait for synchronization
    TC4->COUNT32.CC[0].reg = tickInterval * (UP_CLK / 1024);
    TC4->COUNT32.INTENSET.reg |= TC_INTENSET_MC0;
    NVIC_EnableIRQ(TC4_IRQn); /*Enable TC interrupt*/
}


/*ext_usart handler*/

void irq_handler_tc4(void)
{
    if(TC4->COUNT32.INTFLAG.reg & TC_INTFLAG_MC0)
    {
        printf("TC INT 0x%.2X\r\n", TC4->COUNT32.INTFLAG.reg);
        TC3->COUNT32.INTFLAG.reg |= TC_INTFLAG_MC0;
    }
}