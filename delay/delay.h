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

#define DELAY_BASE_MICRO_SEC 0
#define DELAY_BASE_MILLI_SEC 1

void delaySetup(unsigned int baseTimer);
void delayMs(unsigned int delay);
unsigned long delayGetTicks(void);
unsigned long delayGetDiff(unsigned long start, unsigned long end);
unsigned char delayMicros(unsigned long last, unsigned long target);
