/**************************************************************************//**
 * @file     m032.c
 * @version  V1.00
 * $Revision: 3 $
 * $Date: 18/04/24 3:05p $
 * @brief    M031 series FMC driver source file
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 * Copyright (C) 2021 Nimolabs Ltd.
*****************************************************************************/

#include <stdio.h>
#include "intFlash.h"

/**
  * @brief Disable FMC ISP function.
  * @return None
  */
void intFlashClose(void)
{
    FMC->ISPCTL &= ~FMC_ISPCTL_ISPEN_Msk;
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

    if (u32PageAddr == FMC_SPROM_BASE)
    {
        ret = intFlashErase_SPROM();
    }
    else
    {
        FMC->ISPCMD = FMC_ISPCMD_PAGE_ERASE;
        FMC->ISPADDR = u32PageAddr;
        FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

        while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) { }

        if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk)
        {
            FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;
            ret = -1;
        }
    }
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

    FMC->ISPCMD = FMC_ISPCMD_BANK_ERASE;
    FMC->ISPADDR = u32BankAddr;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) { }

    if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk)
    {
        FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;
        ret = -1;
    }
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

    FMC->ISPCMD = FMC_ISPCMD_PAGE_ERASE;
    FMC->ISPADDR = FMC_SPROM_BASE;
    FMC->ISPDAT = 0x0055AA03UL;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) { }

    if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk)
    {
        FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;
        ret = -1;
    }
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

    FMC->ISPCMD = FMC_ISPCMD_BANK_REMAP;
    FMC->ISPADDR = u32BankIdx;
    FMC->ISPDAT = 0x5AA55AA5UL;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) { }

    if (FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk)
    {
        FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk;
        ret = -1;
    }
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

    if (FMC->ISPCTL & FMC_ISPCTL_BS_Msk)
    {
        ret = 1;
    }

    return ret;
}


/**
  * @brief Enable FMC ISP function
  * @return None
  */
void intFlashOpen(void)
{
    FMC->ISPCTL |=  FMC_ISPCTL_ISPEN_Msk;
}


/**
  * @brief Execute FMC_ISPCMD_READ command to read a word from flash.
  * @param[in]  u32Addr Address of the flash location to be read.
  *             It must be a word aligned address.
  * @return The word data read from specified flash address.
  */
uint32_t intFlashRead(uint32_t u32Addr)
{
    FMC->ISPCMD = FMC_ISPCMD_READ;
    FMC->ISPADDR = u32Addr;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) { }

    return FMC->ISPDAT;
}


/**
  * @brief    Get the base address of Data Flash if enabled.
  * @retval   The base address of Data Flash
  */
uint32_t intFlashReadDataFlashBaseAddr(void)
{
    return FMC->DFBA;
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
    if(i32BootSrc)
    {
        FMC->ISPCTL |= FMC_ISPCTL_BS_Msk; /* Boot from LDROM */
    }
    else
    {
        FMC->ISPCTL &= ~FMC_ISPCTL_BS_Msk;/* Boot from APROM */
    }
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
    FMC->ISPCMD = FMC_ISPCMD_PROGRAM;
    FMC->ISPADDR = u32Addr;
    FMC->ISPDAT = u32Data;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
    while (FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) { }
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

    FMC->ISPCMD  = FMC_ISPCMD_PROGRAM_64;
    FMC->ISPADDR = u32addr;
    FMC->MPDAT0  = u32data0;
    FMC->MPDAT1  = u32data1;
    FMC->ISPTRG  = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) { }

    if (FMC->ISPSTS & FMC_ISPSTS_ISPFF_Msk)
    {
        FMC->ISPSTS |= FMC_ISPSTS_ISPFF_Msk;
        ret = -1;
    }
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

    u32Config[0] = intFlashRead(FMC_CONFIG_BASE);

    if (u32Count > 3UL)
    {
        ret = -1;
    }
    else
    {
        if(u32Count > 1UL)
        {
            u32Config[1] = intFlashRead(FMC_CONFIG_BASE+4UL);
        }
        if(u32Count > 2UL)
        {
            u32Config[2] = intFlashRead(FMC_CONFIG_BASE+8UL);
        }
    }
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
    uint32_t i;

    for (i = 0u; i < u32Count; i++)
    {
        intFlashWrite(FMC_CONFIG_BASE + i * 4u, u32Config[i]);

        if (intFlashRead(FMC_CONFIG_BASE + i * 4u) != u32Config[i])
        {
            ret = -1;
        }
    }

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

    if ((u32addr % 512UL) || (u32count % 512UL))
    {
        ret = 0xFFFFFFFF;
    }
    else
    {
        FMC->ISPCMD  = FMC_ISPCMD_RUN_CKS;
        FMC->ISPADDR = u32addr;
        FMC->ISPDAT  = u32count;
        FMC->ISPTRG  = FMC_ISPTRG_ISPGO_Msk;

        while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) { }

        FMC->ISPCMD = FMC_ISPCMD_READ_CKS;
        FMC->ISPADDR    = u32addr;
        FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

        while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) { }

        ret = FMC->ISPDAT;
    }

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
    uint32_t ret = READ_ALLONE_CMD_FAIL;

    /** Workaround solution for M031 with 512KB Flash uses FMC Read command instead of FMC All-One-Verification command to
      * check the Flash content from 0x40000 to 0x401FF.
      */
    if(u32addr == FMC_APROM_BANK1_BASE)
    {
        uint32_t i;
        u32count = u32count - FMC_CHECKALLONE_UNIT;
        for(i = FMC_APROM_BANK1_BASE; i < (FMC_APROM_BANK1_BASE + FMC_CHECKALLONE_UNIT); i = i+4)
        {
            if( intFlashRead(i) != 0xFFFFFFFF)
                return READ_ALLONE_NOT;
        }

        if(u32count == 0)
            return READ_ALLONE_YES;
        else
            u32addr = u32addr + FMC_CHECKALLONE_UNIT;
    }

    FMC->ISPSTS = 0x80UL; /* clear check all one bit */

    FMC->ISPCMD = FMC_ISPCMD_RUN_ALL1;
    FMC->ISPADDR = u32addr;
    FMC->ISPDAT = u32count;
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

    while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) { }

    do
    {
        FMC->ISPCMD = FMC_ISPCMD_READ_ALL1;
        FMC->ISPADDR = u32addr;
        FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;

        while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) { }
    }
    while (FMC->ISPDAT == 0UL);

    if (FMC->ISPDAT == READ_ALLONE_YES)
    {
        ret = FMC->ISPDAT;
    }

    if (FMC->ISPDAT == READ_ALLONE_NOT)
    {
        ret = FMC->ISPDAT;
    }

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
    (void)(u32Len);
    uint32_t i, idx, u32OnProg, retval = 0;
    int32_t err;

    if ((u32Addr % 8) != 0)
    {
        return -1;
    }

    idx = 0u;
    FMC->ISPCMD = FMC_ISPCMD_MULTI_PROG;
    FMC->ISPADDR = u32Addr;
    retval += 16;
    do
    {
        err = 0;
        u32OnProg = 1u;
        FMC->MPDAT0 = pu32Buf[idx + 0u];
        FMC->MPDAT1 = pu32Buf[idx + 1u];
        FMC->MPDAT2 = pu32Buf[idx + 2u];
        FMC->MPDAT3 = pu32Buf[idx + 3u];
        FMC->ISPTRG = 0x1u;
        idx += 4u;

        for (i = idx; i < (FMC_MULTI_WORD_PROG_LEN / 4u); i += 4u) /* Max data length is 256 bytes (512/4 words)*/
        {
            __set_PRIMASK(1u); /* Mask interrupt to avoid status check coherence error*/
            do
            {
                if ((FMC->MPSTS & FMC_MPSTS_MPBUSY_Msk) == 0u)
                {
                    __set_PRIMASK(0u);

                    FMC->ISPADDR = FMC->MPADDR & (~0xful);
                    idx = (FMC->ISPADDR - u32Addr) / 4u;
                    err = -1;
                }
            }
            while ((FMC->MPSTS & (3u << FMC_MPSTS_D0_Pos)) && (err == 0));

            if (err == 0)
            {
                retval += 8;

                /* Update new data for D0 */
                FMC->MPDAT0 = pu32Buf[i];
                FMC->MPDAT1 = pu32Buf[i + 1u];
                do
                {
                    if ((FMC->MPSTS & FMC_MPSTS_MPBUSY_Msk) == 0u)
                    {
                        __set_PRIMASK(0u);
                        FMC->ISPADDR = FMC->MPADDR & (~0xful);
                        idx = (FMC->ISPADDR - u32Addr) / 4u;
                        err = -1;
                    }
                }
                while ((FMC->MPSTS & (3u << FMC_MPSTS_D2_Pos)) && (err == 0));

                if (err == 0)
                {
                    retval += 8;

                    /* Update new data for D2*/
                    FMC->MPDAT2 = pu32Buf[i + 2u];
                    FMC->MPDAT3 = pu32Buf[i + 3u];
                    __set_PRIMASK(0u);
                }
            }

            if (err < 0)
            {
                break;
            }
        }
        if (err == 0)
        {
            u32OnProg = 0u;
            while (FMC->ISPSTS & FMC_ISPSTS_ISPBUSY_Msk) { }
        }
    }
    while (u32OnProg);
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
    return (FMC->ISPSTS & FMC_ISPSTS_VECMAP_Msk);
}

/**
  * @brief    Read company ID
  * @param    None
  * @return   The company ID (32-bit)
  * @details  The company ID of Nuvoton is fixed to be 0xDA
  */
uint32_t intFlashReadCID(void)
{
    FMC->ISPCMD = FMC_ISPCMD_READ_CID;           /* Set ISP Command Code */
    FMC->ISPADDR = 0x0u;                         /* Must keep 0x0 when read CID */
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;          /* Trigger to start ISP procedure */
#if ISBEN
    __ISB();
#endif                                           /* To make sure ISP/CPU be Synchronized */
    while(FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) {} /* Waiting for ISP Done */

    return FMC->ISPDAT;
}

/**
  * @brief    Read product ID
  * @param    None
  * @return   The product ID (32-bit)
  * @details  This function is used to read product ID.
  */
uint32_t intFlashReadPID(void)
{
    FMC->ISPCMD = FMC_ISPCMD_READ_DID;          /* Set ISP Command Code */
    FMC->ISPADDR = 0x04u;                       /* Must keep 0x4 when read PID */
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;         /* Trigger to start ISP procedure */
#if ISBEN
    __ISB();
#endif                                          /* To make sure ISP/CPU be Synchronized */
    while(FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) {} /* Waiting for ISP Done */

    return FMC->ISPDAT;
}

/**
 * @brief       Read Unique ID
 * @param[in]   u8Index  UID index. 0 = UID[31:0], 1 = UID[63:32], 2 = UID[95:64]
 * @return      The 32-bit unique ID data of specified UID index.
 * @details     To read out 96-bit Unique ID.
 */
uint32_t intFlashReadUID(uint8_t u8Index)
{
    FMC->ISPCMD = FMC_ISPCMD_READ_UID;
    FMC->ISPADDR = ((uint32_t)u8Index << 2u);
    FMC->ISPDAT = 0u;
    FMC->ISPTRG = 0x1u;
#if ISBEN
    __ISB();
#endif
    while(FMC->ISPTRG) {}

    return FMC->ISPDAT;
}

/**
  * @brief      To read UCID
  * @param[in]  u32Index    Index of the UCID to read. u32Index must be 0, 1, 2, or 3.
  * @return     The UCID of specified index
  * @details    This function is used to read unique chip ID (UCID).
  */
uint32_t intFlashReadUCID(uint32_t u32Index)
{
    FMC->ISPCMD = FMC_ISPCMD_READ_UID;            /* Set ISP Command Code */
    FMC->ISPADDR = (0x04u * u32Index) + 0x10u;    /* The UCID is at offset 0x10 with word alignment. */
    FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;           /* Trigger to start ISP procedure */
#if ISBEN
    __ISB();
#endif                                            /* To make sure ISP/CPU be Synchronized */
    while(FMC->ISPTRG & FMC_ISPTRG_ISPGO_Msk) {}  /* Waiting for ISP Done */

    return FMC->ISPDAT;
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
    FMC->ISPCMD = FMC_ISPCMD_VECMAP;  /* Set ISP Command Code */
    FMC->ISPADDR = u32PageAddr;       /* The address of specified page which will be map to address 0x0. It must be page alignment. */
    FMC->ISPTRG = 0x1u;               /* Trigger to start ISP procedure */
#if ISBEN
    __ISB();
#endif                                /* To make sure ISP/CPU be Synchronized */
    while(FMC->ISPTRG) {}             /* Waiting for ISP Done */
}
#if defined ( __CC_ARM )
#pragma arm section

#elif defined ( __GNUC__ )
#pragma GCC pop_options

#endif

/*@}*/ /* end of group FMC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group FMC_Driver */

/*@}*/ /* end of group Standard_Driver */



