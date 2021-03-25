/*
 * Copyright (c) 2015, Alex Taradov <alex@taradov.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


//-----------------------------------------------------------------------------
#define DUMMY __attribute__ ((weak, alias ("irq_handler_dummy")))

//-----------------------------------------------------------------------------
void Reset_Handler(void);
//-----------------------------------------------------------------------------
void irq_handler_dummy(void)
{
    while (1);
}



DUMMY void NMI_Handler(void);
DUMMY void HardFault_Handler(void);
DUMMY void SVC_Handler(void);
DUMMY void PendSV_Handler(void);
DUMMY void SysTick_Handler(void);
DUMMY void BOD_IRQHandler(void);
DUMMY void WDT_IRQHandler(void);
DUMMY void EINT024_IRQHandler(void);
DUMMY void EINT135_IRQHandler(void);
DUMMY void GPABGH_IRQHandler(void);
DUMMY void GPCDEF_IRQHandler(void);
DUMMY void PWM0_IRQHandler(void);
DUMMY void PWM1_IRQHandler(void);
DUMMY void TMR0_IRQHandler(void);
DUMMY void TMR1_IRQHandler(void);
DUMMY void TMR2_IRQHandler(void);
DUMMY void TMR3_IRQHandler(void);
DUMMY void UART02_IRQHandler(void);
DUMMY void UART13_IRQHandler(void);
DUMMY void SPI0_IRQHandler(void);
DUMMY void QSPI0_IRQHandler(void);
DUMMY void ISP_IRQHandler(void);
DUMMY void UART57_IRQHandler(void);
DUMMY void I2C0_IRQHandler(void);
DUMMY void I2C1_IRQHandler(void);
DUMMY void BPWM0_IRQHandler(void);
DUMMY void BPWM1_IRQHandler(void);
DUMMY void USCI01_IRQHandler(void);
DUMMY void USBD_IRQHandler(void);
DUMMY void ACMP01_IRQHandler(void);
DUMMY void PDMA_IRQHandler(void);
DUMMY void UART46_IRQHandler(void);
DUMMY void PWRWU_IRQHandler(void);
DUMMY void ADC_IRQHandler(void);
DUMMY void CKFAIL_IRQHandler(void);
DUMMY void RTC_IRQHandler(void);


extern int main(void);

void __StackTop(void);
extern unsigned int __etext;
extern unsigned int __data_start__;
extern unsigned int __data_end__;
extern unsigned int __bss_start__;
extern unsigned int __bss_end__;

//-----------------------------------------------------------------------------
__attribute__ ((used, section(".vectors")))
void (* const vectors[])(void) =
{
    &__StackTop,                   // 0 - Initial Stack Pointer Value

    // Cortex-M0+ handlers
    Reset_Handler,             // 1 - Reset
    NMI_Handler,               // 2 - NMI
    HardFault_Handler,        // 3 - Hard Fault
    0,                             // 4 - Reserved
    0,                             // 5 - Reserved
    0,                             // 6 - Reserved
    0,                             // 7 - Reserved
    0,                             // 8 - Reserved
    0,                             // 9 - Reserved
    0,                             // 10 - Reserved
    SVC_Handler,           // 11 - SVCall
    0,                             // 12 - Reserved
    0,                             // 13 - Reserved
    PendSV_Handler,           // 14 - PendSV
    SysTick_Handler,          // 15 - SysTick

    // Peripheral handlers
    BOD_IRQHandler,        /*  0: BOD                        */
    WDT_IRQHandler,        /*  1: WDT                        */
    EINT024_IRQHandler,    /*  2: EINT0                      */
    EINT135_IRQHandler,    /*  3: EINT1                      */
    GPABGH_IRQHandler,     /*  4: GPAB                       */
    GPCDEF_IRQHandler,     /*  5: GPCDEF                     */
    PWM0_IRQHandler,       /*  6: PWM0                       */
    PWM1_IRQHandler,       /*  7: PWM1                       */
    TMR0_IRQHandler,       /*  8: TIMER0                     */
    TMR1_IRQHandler,       /*  9: TIMER1                     */
    TMR2_IRQHandler,       /* 10: TIMER2                     */
    TMR3_IRQHandler,       /* 11: TIMER3                     */
    UART02_IRQHandler,     /* 12: UART02                     */
    UART13_IRQHandler,     /* 13: UART13                     */
    SPI0_IRQHandler,       /* 14: SPI0                       */
    QSPI0_IRQHandler,      /* 15: QSPI0                      */
    ISP_IRQHandler,        /* 16: Reserved                   */
    UART57_IRQHandler,     /* 17: UART57                     */
    I2C0_IRQHandler,       /* 18: I2C0                       */
    I2C1_IRQHandler,       /* 19: I2C1                       */
    BPWM0_IRQHandler,      /* 20: BPWM0                      */
    BPWM1_IRQHandler,      /* 21: BPWM1                      */
    USCI01_IRQHandler,     /* 22: USCI01                     */
    USBD_IRQHandler,       /* 23: USBD                       */
    irq_handler_dummy,       /* 24: Reserved                   */
    ACMP01_IRQHandler,     /* 25: ACMP01                     */
    PDMA_IRQHandler,       /* 26: PDMA                       */
    UART46_IRQHandler,     /* 27: UART46                     */
    PWRWU_IRQHandler,      /* 28: PWRWU                      */
    ADC_IRQHandler,        /* 29: ADC                        */
    CKFAIL_IRQHandler,     /* 30: CLK Fail Detect            */
    RTC_IRQHandler        /* 31: RTC                        */
};

//-----------------------------------------------------------------------------
void Reset_Handler(void)
{
    unsigned int *src, *dst;

    src = &__etext;
    dst = &__data_start__;
    while (dst < &__data_end__)
        *dst++ = *src++;

    dst = &__bss_start__;
    while (dst < &__bss_end__)
        *dst++ = 0;

    main();
    while (1);
}

//-----------------------------------------------------------------------------
// void _exit(int status)
// {
//     (void)status;
//     while (1);
// }
