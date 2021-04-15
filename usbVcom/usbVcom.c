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
* File: simpleHid.c
* Description: USB HID API provider
*/
#include <nimolib.h>
#include <stdint.h>

#if defined(__SAMR21) || defined(__SAMD21)
#include "sam.h"
#include "atsamd21/usb_descriptors.c"
#include "atsamd21/usb_std.c"
#include "atsamd21/usb_hid.c"
#elif defined(__NUVO_M032K)
#include "M031Series.h"

#include <string.h>
#include <stdio.h>
#include "NuMicro.h"
#include "m032/m032.h"
#include "m032/usbd.h"
#include "m032/usbd.c"
#include "m032/descriptors.c"
#include "m032/m032.c"
#endif

//-----------------------------------------------------------------------------

