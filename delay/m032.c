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
* File: m032k.c
* Description: Nuvoton M032K delay / SysTick device driver
*/

#include "M031Series.h"
//#include <printf.h>
#include "delay.h"

void delaySetup(unsigned int baseTimer)
{
    unsigned long divisor;
    switch (baseTimer)
    {
    case DELAY_BASE_MICRO_SEC:
        divisor = 1000000;
        break;
    case DELAY_BASE_MILLI_SEC:
        divisor = 1000;
        break;
    default:
        //   printf("Error: delaySetup()\r\n");
        return;
        break;
    }

    SysTick->CTRL = 0;
    SysTick->LOAD = (UP_CLK /2) / divisor;
    SysTick->VAL = 0;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
    sysTickTicks++;
#ifdef NIMOLIB_FREERTOS
    xPortSysTickHandler();
#endif
}