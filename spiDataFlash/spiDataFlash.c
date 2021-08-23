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
* File: spiDataFlash.c
* Description: SPI Dataflash API provider
*/

#include <gpio.h>
#include <nimolib.h>
#include "uart.h"

#if defined(SPI_DATAFLASH_AT25PE80)
#include "at25pe80.c"
#elif defined(SPI_DATAFLASH_SST25VF)
#include "sst25vf.c"
#else
#error SPI_DATAFLASH chip not defined
#endif

#ifndef SPI_DATAFLASH_SIZE
#error SPI_DATAFLASH chip size not defined
#endif