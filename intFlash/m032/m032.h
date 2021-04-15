/**************************************************************************//**
 * @file     m032.h
 * @version  V1.00
 * $Revision: 11 $
 * $Date: 18/06/20 3:38p $
 * @brief    M031 Series Flash Memory Controller Driver Header File
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 * Copyright (C) 2021 Nimolabs Ltd.
 ******************************************************************************/
#ifndef __FMC_H__
#define __FMC_H__

#ifdef __cplusplus
extern "C"
{
#endif

//#define PAGE_SIZE_2048                        /*!< Please enable the compiler option for 2K page size     */

/*----------------------------------------------------------------------------------------------------------*/
/* Define Base Address                                                                                      */
/*----------------------------------------------------------------------------------------------------------*/
#define FMC_APROM_BASE          0x00000000UL    /*!< APROM base address                    \hideinitializer */
#define FMC_LDROM_BASE          0x00100000UL    /*!< LDROM  Base Address                   \hideinitializer */
#define FMC_SPROM_BASE          0x00200000UL    /*!< SPROM  Base Address                   \hideinitializer */
#define FMC_CONFIG_BASE         0x00300000UL    /*!< CONFIG Base Address                   \hideinitializer */
#define FMC_USER_CONFIG_0       0x00300000UL    /*!< User Config 0 address                 \hideinitializer */
#define FMC_USER_CONFIG_1       0x00300004UL    /*!< User Config 1 address                 \hideinitializer */
#define FMC_USER_CONFIG_2       0x00300008UL    /*!< User Config 2 address                 \hideinitializer */

#ifndef PAGE_SIZE_2048
#define FMC_FLASH_PAGE_SIZE     (0x200)         /*!< Flash Page Size (512 bytes)           \hideinitializer */
#define FMC_PAGE_ADDR_MASK      (0xFFFFFE00UL)  /*!< Flash page address mask               \hideinitializer */

#define FMC_SPROM_SIZE          (0x200)         /*!< SPROM Size (512 bytes)                \hideinitializer */
#else
#define FMC_FLASH_PAGE_SIZE     (0x800)         /*!< Flash Page Size (2048 bytes)          \hideinitializer */
#define FMC_PAGE_ADDR_MASK      (0xFFFFF800UL)  /*!< Flash page address mask               \hideinitializer */

#define FMC_SPROM_SIZE          (0x800)         /*!< SPROM Size (2048 bytes)               \hideinitializer */
#endif

#define FMC_MULTI_WORD_PROG_LEN (256UL)         /*!< The length of a multi-word program.   \hideinitializer */

/*----------------------------------------------------------------------------------------------------------*/
/*  ISPCMD constant definitions                                                                             */
/*----------------------------------------------------------------------------------------------------------*/
#define FMC_ISPCMD_READ         0x00UL          /*!< ISP Command: Read flash word          \hideinitializer */
#define FMC_ISPCMD_READ_UID     0x04UL          /*!< ISP Command: Read Unique ID           \hideinitializer */
#define FMC_ISPCMD_READ_ALL1    0x08UL          /*!< ISP Command: Read all-one result      \hideinitializer */																			//  I version
#define FMC_ISPCMD_READ_CID     0x0BUL          /*!< ISP Command: Read Company ID          \hideinitializer */
#define FMC_ISPCMD_READ_DID     0x0CUL          /*!< ISP Command: Read Device ID           \hideinitializer */
#define FMC_ISPCMD_READ_CKS     0x0DUL          /*!< ISP Command: Read checksum            \hideinitializer */
#define FMC_ISPCMD_PROGRAM      0x21UL          /*!< ISP Command: Write flash word         \hideinitializer */
#define FMC_ISPCMD_PROGRAM_64   0x61UL          /*!< ISP Command: 64-bit program Flash     \hideinitializer */
#define FMC_ISPCMD_PAGE_ERASE   0x22UL          /*!< ISP Command: Page Erase Flash         \hideinitializer */
#define FMC_ISPCMD_BANK_ERASE   0x23UL          /*!< ISP Command: Bank Erase Flash         \hideinitializer */
#define FMC_ISPCMD_MULTI_PROG   0x27UL          /*!< ISP Command: Flash Multi-Word Program \hideinitializer */
#define FMC_ISPCMD_RUN_ALL1     0x28UL          /*!< ISP Command: Run all-one verification \hideinitializer */																	// I version
#define FMC_ISPCMD_RUN_CKS      0x2DUL          /*!< ISP Command: Run checksum calculation \hideinitializer */
#define FMC_ISPCMD_BANK_REMAP   0x2CUL          /*!< ISP Command: Bank Remap               \hideinitializer */
#define FMC_ISPCMD_VECMAP       0x2EUL          /*!< ISP Command: Vector Page Remap        \hideinitializer */

#define IS_BOOT_FROM_APROM      0UL             /*!< Is booting from APROM                 \hideinitializer */
#define IS_BOOT_FROM_LDROM      1UL             /*!< Is booting from LDROM                 \hideinitializer */

#define READ_ALLONE_YES         0xA11FFFFFUL    /*!< Check-all-one result is all one.      \hideinitializer */
#define READ_ALLONE_NOT         0xA1100000UL    /*!< Check-all-one result is not all one.  \hideinitializer */
#define READ_ALLONE_CMD_FAIL    0xFFFFFFFFUL    /*!< Check-all-one command failed.         \hideinitializer */
/*@}*/ /* end of group FMC_EXPORTED_CONSTANTS */


/** @addtogroup FMC_EXPORTED_FUNCTIONS FMC Exported Functions
  @{
*/


/*---------------------------------------------------------------------------------------------------------*/
/*  Macros                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/

#define FMC_SET_APROM_BOOT()        (FMC->ISPCTL &= ~FMC_ISPCTL_BS_Msk)         /*!< Select booting from APROM  \hideinitializer */
#define FMC_SET_LDROM_BOOT()        (FMC->ISPCTL |= FMC_ISPCTL_BS_Msk)          /*!< Select booting from LDROM  \hideinitializer */
#define FMC_ENABLE_AP_UPDATE()      (FMC->ISPCTL |=  FMC_ISPCTL_APUEN_Msk)      /*!< Enable APROM update        \hideinitializer */
#define FMC_DISABLE_AP_UPDATE()     (FMC->ISPCTL &= ~FMC_ISPCTL_APUEN_Msk)      /*!< Disable APROM update       \hideinitializer */
#define FMC_ENABLE_CFG_UPDATE()     (FMC->ISPCTL |=  FMC_ISPCTL_CFGUEN_Msk)     /*!< Enable User Config update  \hideinitializer */
#define FMC_DISABLE_CFG_UPDATE()    (FMC->ISPCTL &= ~FMC_ISPCTL_CFGUEN_Msk)     /*!< Disable User Config update \hideinitializer */
#define FMC_ENABLE_LD_UPDATE()      (FMC->ISPCTL |=  FMC_ISPCTL_LDUEN_Msk)      /*!< Enable LDROM update        \hideinitializer */
#define FMC_DISABLE_LD_UPDATE()     (FMC->ISPCTL &= ~FMC_ISPCTL_LDUEN_Msk)      /*!< Disable LDROM update       \hideinitializer */
#define FMC_ENABLE_SP_UPDATE()      (FMC->ISPCTL |=  FMC_ISPCTL_SPUEN_Msk)      /*!< Enable SPROM update        \hideinitializer */
#define FMC_DISABLE_SP_UPDATE()     (FMC->ISPCTL &= ~FMC_ISPCTL_SPUEN_Msk)      /*!< Disable SPROM update       \hideinitializer */
#define FMC_DISABLE_ISP()           (FMC->ISPCTL &= ~FMC_ISPCTL_ISPEN_Msk)      /*!< Disable ISP function       \hideinitializer */
#define FMC_ENABLE_ISP()            (FMC->ISPCTL |=  FMC_ISPCTL_ISPEN_Msk)      /*!< Enable ISP function        \hideinitializer */
#define FMC_GET_FAIL_FLAG()         ((FMC->ISPCTL & FMC_ISPCTL_ISPFF_Msk) ? 1UL : 0UL)  /*!< Get ISP fail flag  \hideinitializer */
#define FMC_CLR_FAIL_FLAG()         (FMC->ISPCTL |= FMC_ISPCTL_ISPFF_Msk)       /*!< Clear ISP fail flag        \hideinitializer */
#define FMC_ENABLE_ISP_INT()        (FMC->ISPCTL |=  FMC_ISPCTL_INTEN_Msk) /*!< Enable ISP interrupt */
#define FMC_DISABLE_ISP_INT()       (FMC->ISPCTL &= ~FMC_ISPCTL_INTEN_Msk) /*!< Disable ISP interrupt */
#define FMC_GET_ISP_INT_FLAG()      ((FMC->ISPSTS & FMC_ISPSTS_INTFLAG_Msk) ? 1UL : 0UL) /*!< Get ISP interrupt flag Status */
#define FMC_CLEAR_ISP_INT_FLAG()    (FMC->ISPSTS = FMC_ISPSTS_INTFLAG_Msk) /*!< Clear ISP interrupt flag*/

/*---------------------------------------------------------------------------------------------------------*/

uint32_t intFlashReadCID(void);
uint32_t intFlashReadPID(void);
uint32_t intFlashReadUID(uint8_t u8Index);
uint32_t intFlashReadUCID(uint32_t u32Index);
void intFlashSetVectorPageAddr(uint32_t u32PageAddr);
uint32_t intFlashGetVECMAP(void);




/*---------------------------------------------------------------------------------------------------------*/
/*  Functions                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/



/*@}*/ /* end of group FMC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group FMC_Driver */

/*@}*/ /* end of group Standard_Driver */

#ifdef __cplusplus
}
#endif

#endif   /* __FMC_H__ */

