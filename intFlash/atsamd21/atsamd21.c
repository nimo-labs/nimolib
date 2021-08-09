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

#include <stdio.h>
#include "intFlash.h"

/**
  * @brief Disable FMC ISP function.
  * @return None
  */
void intFlashClose(void)
{

}


/**
  * @brief Execute FMC_ISPCMD_PAGE_ERASE command to erase a flash page. The page size is 4096 bytes.
  * @param[in]  u32PageAddr Address of the flash page to be erased.
  *             It must be a 4096 bytes aligned address.
  * @return ISP page erase success or not.
  * @retval   0  Success
  * @retval   -1  Erase failed
  */
int32_t intFlashErase(uint32_t u32PageAddr)
{
    int32_t  ret = 0;


    return ret;
}

/**
 * @brief      Execute Flash Bank erase
 *
 * @param[in]  u32BankAddr Base address of the flash bank to be erased.
 *
 * @return     ISP bank erase success or not.
 * @retval     0  Success
 * @retval     -1  Erase failed
 *
 * @details  Execute FMC_ISPCMD_BANK_ERASE command to erase a flash bank.
 */
int32_t intFlashErase_Bank(uint32_t u32BankAddr)
{
    int32_t ret = 0;

    return ret;
}

/**
  * @brief Execute FMC_ISPCMD_PAGE_ERASE command to erase SPROM. The page size is 4096 bytes.
  * @return   SPROM page erase success or not.
  * @retval   0  Success
  * @retval   -1  Erase failed
  */
int32_t intFlashErase_SPROM(void)
{
    int32_t  ret = 0;

    return ret;
}

/**
 * @brief Execute FMC_ISPCMD_BANK_REMAP command to remap bank.
 * @return   Bank remap success or not.
 * @retval   0  Success
 * @retval   -1  Erase failed
 */
int32_t intFlashRemapBank(uint32_t u32BankIdx)
{
    int32_t ret = 0;

    return ret;
}

/**
  * @brief Get the current boot source.
  * @return The current boot source.
  * @retval   0  Is boot from APROM.
  * @retval   1  Is boot from LDROM.
  */
int32_t intFlashGetBootSource (void)
{
    int32_t  ret = 0;


    return ret;
}


/**
  * @brief Enable FMC ISP function
  * @return None
  */
void intFlashOpen(void)
{

}


/**
  * @brief Execute FMC_ISPCMD_READ command to read a word from flash.
  * @param[in]  u32Addr Address of the flash location to be read.
  *             It must be a word aligned address.
  * @return The word data read from specified flash address.
  */
uint32_t intFlashRead(uint32_t u32Addr)
{

}


/**
  * @brief    Get the base address of Data Flash if enabled.
  * @retval   The base address of Data Flash
  */
uint32_t intFlashReadDataFlashBaseAddr(void)
{

}

/**
  * @brief      Set boot source from LDROM or APROM after next software reset
  * @param[in]  i32BootSrc
  *                1: Boot from LDROM
  *                0: Boot from APROM
  * @return    None
  * @details   This function is used to switch APROM boot or LDROM boot. User need to call
  *            FMC_SetBootSource to select boot source first, then use CPU reset or
  *            System Reset Request to reset system.
  */
void intFlashSetBootSource(int32_t i32BootSrc)
{

}

/**
  * @brief Execute ISP FMC_ISPCMD_PROGRAM to program a word to flash.
  * @param[in]  u32Addr Address of the flash location to be programmed.
  *             It must be a word aligned address.
  * @param[in]  u32Data The word data to be programmed.
  * @return None
  */
void intFlashWrite(uint32_t u32Addr, uint32_t u32Data)
{

}

/**
  * @brief Execute ISP FMC_ISPCMD_PROGRAM_64 to program a double-word to flash.
  * @param[in]  u32addr Address of the flash location to be programmed.
  *             It must be a double-word aligned address.
  * @param[in]  u32data0   The word data to be programmed to flash address u32addr.
  * @param[in]  u32data1   The word data to be programmed to flash address u32addr+4.
  * @return   0   Success
  * @return   -1  Failed
  */
int32_t intFlashWrite8Bytes(uint32_t u32addr, uint32_t u32data0, uint32_t u32data1)
{
    int32_t  ret = 0;


    return ret;
}

/**
  * @brief Execute FMC_ISPCMD_READ command to read User Configuration.
  * @param[out]  u32Config A three-word array.
  *              u32Config[0] holds CONFIG0, while u32Config[1] holds CONFIG1.
  * @param[in] u32Count Available word count in u32Config.
  * @return Success or not.
  * @retval   0  Success.
  * @retval   -1  Invalid parameter.
  */
int32_t intFlashReadConfig(uint32_t u32Config[], uint32_t u32Count)
{
    int32_t   ret = 0;


    return ret;
}

/**
 * @brief Execute ISP commands to erase then write User Configuration.
 * @param[in] u32Config   A two-word array.
 *            u32Config[0] holds CONFIG0, while u32Config[1] holds CONFIG1.
 * @param[in] u32Count  Always be 2 in this BSP.
 * @return Success or not.
 * @retval   0  Success.
 * @retval   -1  Invalid parameter.
 */
int32_t intFlashWriteConfig(uint32_t u32Config[], uint32_t u32Count)
{
    int32_t ret = 0;


    return ret;
}

/**
  * @brief Run CRC32 checksum calculation and get result.
  * @param[in] u32addr   Starting flash address. It must be a page aligned address.
  * @param[in] u32count  Byte count of flash to be calculated. It must be multiple of 512 bytes.
  * @return Success or not.
  * @retval   0           Success.
  * @retval   0xFFFFFFFF  Invalid parameter.
  */
uint32_t  intFlashGetChkSum(uint32_t u32addr, uint32_t u32count)
{
    uint32_t   ret;



    return ret;
}

/**
 * @brief Run flash all one verification and get result.
 *
 * @param[in] u32addr   Starting flash address. It must be a page aligned address.
 * @param[in] u32count  Byte count of flash to be calculated. It must be multiple of 512 bytes.
 *
 * @retval   READ_ALLONE_YES      The contents of verified flash area are 0xFFFFFFFF.
 * @retval   READ_ALLONE_NOT  Some contents of verified flash area are not 0xFFFFFFFF.
 * @retval   READ_ALLONE_CMD_FAIL  Unexpected error occurred.
 *
 * @details  Run ISP check all one command to check specify area is all one or not.
 */
#define FMC_APROM_BANK1_BASE    (0x40000)
#define FMC_CHECKALLONE_UNIT    (512)
uint32_t intFlashCheckAllOne(uint32_t u32addr, uint32_t u32count)
{
    uint32_t ret = 0;//READ_ALLONE_CMD_FAIL;


    return ret;
}

/**
 * @brief      Write Multi-Word bytes to flash
 *
 * @param[in]  u32Addr    Start flash address in APROM where the data chunk to be programmed into.
 *                        This address must be 8-bytes aligned to flash address.
 * @param[in]  pu32Buf    Buffer that carry the data chunk.
 * @param[in]  u32Len     Length of the data chunk in bytes.
 *
 * @retval     >=0  Number of data bytes were programmed.
 * @return     -1   Invalid address.
 *
 * @detail     Program Multi-Word data into specified address of flash.
 */
#if defined ( __CC_ARM )
#pragma arm section code="fastcode"
int32_t intFlashWriteMultiple(uint32_t u32Addr, uint32_t pu32Buf[], uint32_t u32Len)

#elif defined ( __ICCARM__ )
int32_t intFlashWriteMultiple(uint32_t u32Addr, uint32_t pu32Buf[], uint32_t u32Len) @ "fastcode"

#elif defined ( __GNUC__ )
#pragma GCC push_options
#pragma GCC optimize ("O0")
__attribute__ ((used, long_call, section(".fastcode"))) int32_t intFlashWriteMultiple(uint32_t u32Addr, uint32_t pu32Buf[], uint32_t u32Len)

#else
int32_t intFlashWriteMultiple(uint32_t u32Addr, uint32_t pu32Buf[], uint32_t u32Len)
#endif
{

    uint32_t i, idx, u32OnProg, retval = 0;

    return retval;
}

/**
 * @brief       Get current vector mapping address.
 * @param       None
 * @return      The current vector mapping address.
 * @details     To get VECMAP value which is the page address for remapping to vector page (0x0).
 * @note
 *              VECMAP only valid when new IAP function is enabled. (CBS = 10'b or 00'b)
 */
uint32_t intFlashGetVECMAP(void)
{

}

/**
  * @brief    Read company ID
  * @param    None
  * @return   The company ID (32-bit)
  * @details  The company ID of Nuvoton is fixed to be 0xDA
  */
uint32_t intFlashReadCID(void)
{

}

/**
  * @brief    Read product ID
  * @param    None
  * @return   The product ID (32-bit)
  * @details  This function is used to read product ID.
  */
uint32_t intFlashReadPID(void)
{

}

/**
 * @brief       Read Unique ID
 * @param[in]   u8Index  UID index. 0 = UID[31:0], 1 = UID[63:32], 2 = UID[95:64]
 * @return      The 32-bit unique ID data of specified UID index.
 * @details     To read out 96-bit Unique ID.
 */
uint32_t intFlashReadUID(uint8_t u8Index)
{

}

/**
  * @brief      To read UCID
  * @param[in]  u32Index    Index of the UCID to read. u32Index must be 0, 1, 2, or 3.
  * @return     The UCID of specified index
  * @details    This function is used to read unique chip ID (UCID).
  */
uint32_t intFlashReadUCID(uint32_t u32Index)
{

}

/**
 * @brief       Set vector mapping address
 * @param[in]   u32PageAddr  The page address to remap to address 0x0. The address must be page alignment.
 * @return      To set VECMAP to remap specified page address to 0x0.
 * @details     This function is used to set VECMAP to map specified page to vector page (0x0).
 * @note
 *              VECMAP only valid when new IAP function is enabled. (CBS = 10'b or 00'b)
 */
void intFlashSetVectorPageAddr(uint32_t u32PageAddr)
{

}
#if defined ( __CC_ARM )
#pragma arm section

#elif defined ( __GNUC__ )
#pragma GCC pop_options

#endif

/*@}*/ /* end of group FMC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group FMC_Driver */

/*@}*/ /* end of group Standard_Driver */



