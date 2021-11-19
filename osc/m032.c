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
* Description: Nuvoton M032 oscillator device driver
*/

#include "NuMicro.h"

void oscSet(unsigned char oscillator)
{
    SYS_UnlockReg();

    switch(oscillator)
    {
    case OSC_HXTEN:
        CLK->PWRCTL = CLK_PWRCTL_HXTEN_Msk;
        break;
    case OSC_LXTEN:
        CLK->PWRCTL = CLK_PWRCTL_LXTEN_Msk;
        break;
    case OSC_HIRCEN:
        CLK->PWRCTL = CLK_PWRCTL_HIRCEN_Msk;
        break;
    case OSC_LIRCEN:
        CLK->PWRCTL = CLK_PWRCTL_LIRCEN_Msk;
        break;
    }
    SYS_LockReg();
}