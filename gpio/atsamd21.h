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
* File: atsamd21.h
* Description: SAMD21 GPIO device driver
*/

#define GPIO_PIN_DIR(lPORT, PIN, DIR)                     \
	if (GPIO_DIR_OUT == DIR)                              \
	{													\
		PORT->Group[lPORT].DIR.reg |= (1 << PIN);      \
		(PORT->Group[lPORT].PINCFG[PIN].reg &= ~(1 << 1)); \
	}														\
	else                                                  \
	{                                                     \
		(PORT->Group[lPORT].DIR.reg &= ~(1 << PIN));   \
		(PORT->Group[lPORT].PINCFG[PIN].reg |= (1 << 1)); \
	}
#define GPIO_PIN_OUT(lPORT, PIN, VAL)               \
	if (GPIO_OUT_HIGH == VAL)                       \
		(PORT->Group[lPORT].OUT.reg |= (1 << PIN)); \
	else                                            \
		(PORT->Group[lPORT].OUT.reg &= ~(1 << PIN))
#define GPIO_PIN_TGL(lPORT, PIN) (PORT->Group[lPORT].OUT.reg ^= (1 << PIN))
#define GPIO_PIN_READ(lPORT, PIN) (PORT->Group[lPORT].IN.reg & (1 << PIN))