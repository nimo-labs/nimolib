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
* File: at25pe80.h
* Description: AT25PE80 dataflash chip device driver
*/

/*Chip commands*/
/*Read commands*/
#define AT25PE80_CONTINUOUS_READ_SLOW 0x03

/*Program / erase commands*/
#define AT25PE80_PROG_RMW 0x58
