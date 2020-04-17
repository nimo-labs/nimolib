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
* File: utils.h
* Description: ATSAMD USB HID device driver
*/

#ifndef _UTILS_H_
#define _UTILS_H_

/*- Definitions -------------------------------------------------------------*/
#define PACK __attribute__((packed))

#define INLINE static inline __attribute__((always_inline))

#define CONFIG(x) __attribute__((used, section(".config." #x))) static volatile const uint32_t x

#define LIMIT(a, b) (((a) > (b)) ? (b) : (a))

#endif // _UTILS_H_
