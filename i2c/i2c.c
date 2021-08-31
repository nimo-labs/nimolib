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
* File: atsamd21.c
* Description: ATSAMD21 SPI device driver
*/

/** Pings the selected address to see if a device is present.
 *
 * Returns 0 if device present
 * Returns 1 if device NOT present
 *
 */

#include <system.h>
#include <nimolib.h>
#include <printf.h>
#include "i2c.h"

#if defined(__SAMD21)
#include <sam.h>
#include "samd/i2c_samd.c"
#elif defined(__SAMR21)
#include <sam.h>
#include "samd/i2c_samd.c"
#elif defined(__XMEGA)
#include "xmega/i2c_xmega.c"
#elif defined(__NUVO_M032K)
#include "m032/i2c_m032.c"
#else
#error uC not defined
#endif

unsigned char i2cPing(unsigned char chan, unsigned char addr)
{
    return i2cWrite(chan, addr, (unsigned char *)"\0", 0, TRUE);
}

void i2cDetect(__attribute__((unused)) unsigned char (chan))
{
    unsigned int i, j, res;
    unsigned int first = 0x03, last = 0x77;

    printf("        0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\r\n");

    for (i = 0; i < 127; i += 16)
    {
        printf("0x%.2X", i);
        printf(": ");
        for (j = 0; j < 16; j++)
        {
            /* Skip unwanted addresses */
            if (i + j < first || i + j > last)
            {
                printf("     ");
                continue;
            }

            /* Probe this address */
            res = i2cPing(0, i + j);

            if (res)
                printf(" --  ");
            else
            {
                printf(" %.2X", (i + j));
                printf("  ");
            }
        }
        printf("\r\n");
    }
}
