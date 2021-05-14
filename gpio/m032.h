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
* File: m032k.h
* Description: Nuvoton M032K GPIO device driver
*/

#include "M031Series.h"

#define GPIO_PORTA PA
#define GPIO_PORTB PB
#define GPIO_PORTC PC
#define GPIO_PORTD PD
#define GPIO_PORTE PE
#define GPIO_PORTF PF
#define GPIO_PORTG PG

#define GPIO_PIN_DIR(lPORT, PIN, lDIR)                     \
	if (GPIO_DIR_OUT == lDIR)                              \
	{													\
		lPORT->MODE |= (1 << (PIN * 2));      \
	}														\
	else                                                  \
	{                                                     \
		lPORT->MODE &= ~(3 << (PIN * 2));   \
	}
#define GPIO_PIN_OUT(lPORT, PIN, VAL)               \
	if (GPIO_OUT_HIGH == VAL)                       \
		lPORT->DOUT |= (1 << PIN); \
	else                                            \
		lPORT->DOUT &= ~(1 << PIN);
#define GPIO_PIN_TGL(lPORT, PIN) (lPORT->DOUT ^= (1 << PIN))
#define GPIO_PIN_READ(lPORT, lPIN) ((lPORT->PIN >> lPIN) & 0x1)