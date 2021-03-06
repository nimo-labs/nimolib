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
* File: spi.c
* Description: ATSAMD21 SPI API provider
*/


//#include <system.h>
#include <nimolib.h>
#include "spi.h"

#if defined(__SAMR21)
#include <sam.h>
#if defined(SPI_USE_BB)
#include "samSpiBB.c"
#else
#include "spi_samd.c"
#endif
#elif defined(__SAMD21)
#include <sam.h>
#if defined(SPI_USE_BB)
#include "samSpiBB.c"
#else
#include "spi_samd.c"
#endif
#elif defined(__NUVO_M032K)
#include "NuMicro.h"
#include "m032.c"
#else
#include "i2c_xmega.c"

#endif
