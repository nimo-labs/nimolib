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
* File: gpio.c
* Description: SAMD21 GPIO API Provider
*/

#define GPIO_DIR_IN 0
#define GPIO_DIR_OUT 1

#define GPIO_OUT_LOW 0
#define GPIO_OUT_HIGH 1

#if defined(__SAMR21)
#include "atsamd21.h"
#elif defined(__SAMD21)
#include "atsamd21.h"
#elif defined(__NUVO_M032K)
#include "m032.h"
#endif
