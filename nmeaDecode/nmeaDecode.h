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
* File: nmeaDecode.h
* Description: NMEA GPS decoding library
*/

#define NMEA_DECODE_TALKER_GPS 0
#define NMEA_DECODE_TALKER_GLONASS 1

struct s_nmeaDecode
{
    char talker[7];
    char time[10];
    char gpsLat[14];
    char latLoc;
    char gpsLong[14];
    char longLoc;
    char qual;
    char gpsAlt[8];
    char altUnit;
};

unsigned char nmeaDecode(struct s_nmeaDecode *view, unsigned char data);
unsigned char nmeaTalkerDecode(char *out, char *in);