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
* File: intFlash.c
* Description: NIMOLabs ATSAMD21 Internal Flash driver
*/

#define INT_FLASH_PAGE_SIZE 2048

#define PAGES_IN_ERASE_BLOCK NVMCTRL_ROW_PAGES
#define ERASE_BLOCK_SIZE NVMCTRL_ROW_SIZE
#define FLASH_PAGE_SIZE_WORDS (int)(FLASH_PAGE_SIZE / sizeof(uint32_t))