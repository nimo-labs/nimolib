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
* File: nmeaDecode.c
* Description: NMEA GPS decoding library
*/

#include <string.h>
#include "nmeaDecode.h"
#include "nmeaDefines.h"

#define STATE_BUFFER 0
#define STATE_DECODE 1

static unsigned char state = STATE_BUFFER;

static char localBuff[150];
static unsigned int buffCtr = 0;

char *nimotok(char *str, char delim)
{
    static char *strStart;
    static char *origLen;

    if (NULL != str)
    {
        strStart = str;
        origLen = str + strlen(str);
    }
    else
        strStart = strStart + strlen(strStart) + 1;
    if (strStart >= origLen)
        return 0;

    for (char *i = strStart; i < (strStart + strlen(strStart)); i++)
    {
        if (*i == delim)
        {
            *i = '\0';
            return strStart;
        }
    }
    return 0;
}

unsigned char nmeaDecode(struct s_nmeaDecode *view, unsigned char data)
{
    char *token;

    if (STATE_BUFFER == state)
    {
        switch (data)
        {
        case '\r':
            break;
        case '\n':
            localBuff[buffCtr + 1] = '\0';
            buffCtr = 0;
            state = STATE_DECODE;
            break;
        default:
            if (buffCtr < sizeof(localBuff))
            {
                localBuff[buffCtr] = data;
                buffCtr++;
            }
            break;
        }
    }
    if (STATE_DECODE == state)
    {
        token = nimotok(localBuff, ',');
        if (token)
        {
            if (0 == strncmp("GGA", (char *)&token[3], 3))
            {
                strcpy(view->talker, token);
                token = nimotok(NULL, ','); /*Get time*/
                if (token)
                    strcpy(view->time, token);
                token = nimotok(NULL, ','); /*Get Lat*/
                if (token)
                    strcpy(view->gpsLat, token);
                token = nimotok(NULL, ','); /*Get Lat loc*/
                if (token)
                    view->latLoc = token[0];
                token = nimotok(NULL, ','); /*Get Long*/
                if (token)
                    strcpy(view->gpsLong, token);
                token = nimotok(NULL, ','); /*Get Long loc*/
                if (token)
                    view->longLoc = token[0];
                token = nimotok(NULL, ','); /*Get Qual*/
                if (token)
                    view->qual = *token;
                token = nimotok(NULL, ','); /*Get num sat*/
                if(token)
                    view->numSats = *token;
                token = nimotok(NULL, ','); /*Get HDOP*/
                token = nimotok(NULL, ','); /*Get Alt*/
                if (token)
                    strcpy(view->gpsAlt, token);
                token = nimotok(NULL, ','); /*Get Alt unit*/
                if (token)
                    view->altUnit = *token;
            }
        }
        state = STATE_BUFFER;
        return 1; /*Data decoded*/
    }
    return 0;
}

/*Note that the lenght of out must be at least 8 bytes!*/
unsigned char nmeaTalkerDecode(char *out, char *in)
{
    if (0 == strncmp("$GA", (char *)in, 3))
        strcpy(out, "Galileo");
    else if (0 == strncmp("$GB", (char *)in, 3))
        strcpy(out, "BeiDou");
    else if (0 == strncmp("$GI", (char *)in, 3))
        strcpy(out, "NavIC");
    else if (0 == strncmp("$GL", (char *)in, 3))
        strcpy(out, "GLONASS");
    else if (0 == strncmp("$GN", (char *)in, 3))
        strcpy(out, "Generic");
    else if (0 == strncmp("$GP", (char *)in, 3))
        strcpy(out, "GPS");
    else if (0 == strncmp("$GQ", (char *)in, 3))
        strcpy(out, "QZSS");
    else
        return 1;
    return 0;
}