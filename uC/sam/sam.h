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
* File: sam.h
* Description: ATSAMD API provider
*/

#if defined(__SAMR21)
#include "samr21.h"
#include "nvmData.h"
#elif defined(__SAMD21)
#include "samd21.h"
#include "nvmData.h"
#else
#error No MCU family defined
#endif

/*- Definitions -------------------------------------------------------------*/
#define SAM_GPIO_PORTA 0
#define SAM_GPIO_PORTB 1
#define SAM_GPIO_PORTC 2

#define SAM_GPIO_PMUX_A 0
#define SAM_GPIO_PMUX_B 1
#define SAM_GPIO_PMUX_C 2
#define SAM_GPIO_PMUX_D 3
#define SAM_GPIO_PMUX_E 4
#define SAM_GPIO_PMUX_F 5
#define SAM_GPIO_PMUX_G 6
#define SAM_GPIO_PMUX_H 7
#define SAM_GPIO_PMUX_I 8

#define SAM_GPIO_MUXE 0
#define SAM_GPIO_MUXO 4

#define TRUE 1
#define FALSE 0

#define UNUSED(x) UNUSED_##x __attribute__((__unused__))

#define PIN_SET_OUT(PORT_def, PIN) (PORT->Group[PORT_def].DIRSET.reg |= (1 << PIN))
#define PIN_SET_IN(PORT_def, PIN) (PORT->Group[PORT_def].DIRSET.reg &= ~(1 << PIN))

#define PIN_SET_HIGH(PORT_def, PIN) (PORT->Group[PORT_def].OUT.reg |= (1 << PIN))
#define PIN_SET_LOW(PORT_def, PIN) (PORT->Group[PORT_def].OUT.reg &= (1 << PIN))
#define PIN_SET_TGL(PORT_def, PIN) (PORT->Group[PORT_def].OUT.reg ^= (1 << PIN))
