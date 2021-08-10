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
* File: m032k.c
* Description: Nuvoton M032K sysCore device driver
*/

#include "M031Series.h"

void sysCoreuCReset(void)
{
    SYS->IPRST0 = SYS_IPRST0_CHIPRST_Msk;
}