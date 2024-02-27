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
* File: delay.c
* Description: SAMD21 delay / SysTick API provider
*/

#include <nimolib.h>

static volatile unsigned long sysTickTicks = 0;

#if defined(__SAMR21)
#include "atsamd21.c"
#elif defined(__SAMD21)
#include "atsamd21.c"
#elif defined(__NUVO_M032K)
#include "m032.c"
#endif

unsigned long delayGetDiff(unsigned long start, unsigned long end)
{
    if (end > start)
    {
        return (end - start);
    }
    else
    {
        return sysTickTicks - (start + 4294967295);
    }
}

unsigned long delayGetTicks(void)
{
    return sysTickTicks;
}

unsigned char delayMillis(unsigned long last, unsigned long target)
{
    if (delayGetDiff(last, sysTickTicks) > target)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void delayMs(unsigned int delay)
{
    unsigned long ticks;

    ticks = delayGetTicks();
    while (delayGetDiff(ticks, delayGetTicks()) < delay)
        ;
}