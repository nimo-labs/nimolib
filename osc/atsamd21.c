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
* Description: ATSAMD21 oscillator device driver
*/

#include <sam.h>
#include <nimolib.h>

#include "osc.h"
void oscSet(unsigned char oscillator)
{
    uint32_t coarse, fine;

    switch (oscillator)
    {
    case OSC_INT8:
        SYSCTRL->OSC8M.bit.PRESC = 0;
        break;

    case OSC_48DFLL:
        /*Setup 48Mhz DFLL */

        SYSCTRL->OSC8M.bit.PRESC = 0;

        NVMCTRL->CTRLB.bit.RWS = 2;

        coarse = NVM_READ_CAL(DFLL48M_COARSE_CAL);
        fine = NVM_READ_CAL(DFLL48M_FINE_CAL);

        SYSCTRL->DFLLCTRL.reg = 0; // See Errata 9905 */
        while (0 == (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY))
            ;

        SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_MUL(48000);
        SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE(coarse) | SYSCTRL_DFLLVAL_FINE(fine);

        SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE | SYSCTRL_DFLLCTRL_USBCRM |
                                SYSCTRL_DFLLCTRL_BPLCKC | SYSCTRL_DFLLCTRL_CCDIS | SYSCTRL_DFLLCTRL_STABLE;

        while (0 == (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY))
            ;

        GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(0) | GCLK_GENCTRL_SRC(GCLK_SOURCE_DFLL48M) |
                            GCLK_GENCTRL_RUNSTDBY | GCLK_GENCTRL_GENEN;
        while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
            ;
        break;
    }
    // Enable interrupts
    asm volatile("cpsie i");
}