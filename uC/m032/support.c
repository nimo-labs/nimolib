#include "NuMicro.h"

void SYS_UnlockReg(void)
{
    do
    {
        SYS->REGLCTL = 0x59;
        SYS->REGLCTL = 0x16;
        SYS->REGLCTL = 0x88;
    }
    while (SYS->REGLCTL == 0);
}

void SYS_LockReg(void)
{
    SYS->REGLCTL = 0;
}