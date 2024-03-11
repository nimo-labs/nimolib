/*
* Copyright 2024 Nicholas Dark.
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
* File: pwm.c
* Description: PWM API provider
*/

#include "pwm.h"

#if defined(__NUVO_M032K)
#include "m032/m032.c"
#else
#error PWM: Microcontroller not supported.
#endif