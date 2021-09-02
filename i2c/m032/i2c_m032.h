/*---------------------------------------------------------------------------------------------------------*/
/* USCI_I2C master event definitions                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
enum UI2C_MASTER_EVENT
{
    MASTER_SEND_ADDRESS = 10u,    /*!< Master send address to Slave */
    MASTER_SEND_H_WR_ADDRESS,    /*!< Master send High address to Slave */
    MASTER_SEND_H_RD_ADDRESS,    /*!< Master send address to Slave (Read ADDR) */
    MASTER_SEND_L_ADDRESS,       /*!< Master send Low address to Slave */
    MASTER_SEND_DATA,            /*!< Master Send Data to Slave */
    MASTER_SEND_REPEAT_START,    /*!< Master send repeat start to Slave */
    MASTER_READ_DATA,            /*!< Master Get Data from Slave */
    MASTER_STOP,                 /*!< Master send stop to Slave */
    MASTER_SEND_START            /*!< Master send start to Slave */
};

/*---------------------------------------------------------------------------------------------------------*/
/*  USCI_CTL constant definitions.                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#define UI2C_CTL_PTRG              0x20UL    /*!< USCI_CTL setting for I2C control bits. It would set PTRG bit \hideinitializer */
#define UI2C_CTL_STA               0x08UL    /*!< USCI_CTL setting for I2C control bits. It would set STA bit \hideinitializer */
#define UI2C_CTL_STO               0x04UL    /*!< USCI_CTL setting for I2C control bits. It would set STO bit \hideinitializer */
#define UI2C_CTL_AA                0x02UL    /*!< USCI_CTL setting for I2C control bits. It would set AA bit  \hideinitializer */

#define UI2C_START(ui2c) ((ui2c)->PROTCTL = ((ui2c)->PROTCTL & ~UI2C_PROTCTL_PTRG_Msk) | UI2C_PROTCTL_STA_Msk)
#define UI2C_STOP(ui2c) ((ui2c)->PROTCTL = ((ui2c)->PROTCTL & ~0x2E) | (UI2C_PROTCTL_PTRG_Msk | UI2C_PROTCTL_STO_Msk))
#define UI2C_GET_PROT_STATUS(ui2c)    ((ui2c)->PROTSTS)
#define UI2C_CLR_PROT_INT_FLAG(ui2c,u32IntTypeFlag)    ((ui2c)->PROTSTS = (u32IntTypeFlag))
#define UI2C_SET_CONTROL_REG(ui2c, u8Ctrl) ((ui2c)->PROTCTL = ((ui2c)->PROTCTL & ~0x2EU) | (u8Ctrl))
#define UI2C_GET_DATA(ui2c) ((ui2c)->RXDAT)
#define UI2C_SET_DATA(ui2c, u8Data) ((ui2c)->TXDAT = (u8Data))