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
* File: uart.h
* Description: ATSAMD UART API provider
*/
#include <stdint.h>

#if defined(__NUVO_M032K)
#include "NuMicro.h"
#include "m032/m032.h"
#elif defined(__SAMR21) || defined(__SAMD21)
#include "atsamd21/atsamd21.h"
#endif

void intFlashClose(void);
int32_t intFlashErase(uint32_t u32PageAddr);
int32_t intFlashErase_SPROM(void);
int32_t intFlashErase_Bank(uint32_t u32BankAddr);
int32_t intFlashGetBootSource(void);
void intFlashOpen(void);
uint32_t intFlashRead(uint32_t u32Addr);
uint32_t intFlashReadDataFlashBaseAddr(void);
void intFlashSetBootSource(int32_t i32BootSrc);
void intFlashWrite(uint32_t u32Addr, uint32_t u32Data);
int32_t  intFlashWrite8Bytes(uint32_t u32addr, uint32_t u32data0, uint32_t u32data1);
int32_t  intFlashReadConfig(uint32_t u32Config[], uint32_t u32Count);
int32_t  intFlashWriteConfig(uint32_t u32Config[], uint32_t u32Count);
uint32_t intFlashGetChkSum(uint32_t u32addr, uint32_t u32count);
uint32_t  intFlashCheckAllOne(uint32_t u32addr, uint32_t u32count);
int32_t intFlashWriteMultiple(uint32_t u32Addr, uint32_t pu32Buf[], uint32_t u32Len);
int32_t intFlashRemapBank(uint32_t u32BankIdx);

