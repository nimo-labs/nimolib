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

    PM->APBCMASK.reg |= PM_APBCMASK_TC3;

    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(TC3_GCLK_ID) |
                        GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(PERHIP_CLK_GEN);

    TC3->COUNT32.CTRLA.reg = 0;
    TC3->COUNT32.CTRLA.reg |= TC_CTRLA_PRESCSYNC_GCLK | TC_CTRLA_RUNSTDBY | TC_CTRLA_PRESCALER_DIV1024 |
                              TC_CTRLA_WAVEGEN_NFRQ | TC_CTRLA_MODE_COUNT32 | TC_CTRLA_ENABLE;
    TC3->COUNT32.CC[0].reg = tickInterval * 46875;
    TC3->COUNT32.INTENSET.reg |= TC_INTENCLR_MC0;
    NVIC_EnableIRQ(TC3_IRQn); /*Enable SERCOM interrupt*/
}


/*ext_usart handler*/

void irq_handler_tc3(void)
{
    if(TC3->COUNT32.INTFLAG.reg & TC_INTFLAG_MC0)
    {
        printf("TC INT\r\n");
        TC3->COUNT32.INTFLAG.reg |= TC_INTFLAG_MC0;
    }
}