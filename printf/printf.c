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
* File: printf.c
* Description: Simple printf library
*/

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#include <nimolib.h>
#if (PRINTF_BUFF_SIZE > 0)
#include <uart.h>
#endif

#if (PRINTF_BUFF_SIZE == 0)
int printf(__attribute__((unused)) const char *format, ...)
{
#else
int printf(const char *format, ...)
{
    char tbuff[PRINTF_BUFF_SIZE];
    va_list aptr;
    int ret;

    va_start(aptr, format);
    ret = vsnprintf(&tbuff[0], sizeof(tbuff), format, aptr);
    va_end(aptr);
    if ((unsigned int)ret > sizeof(tbuff) - 1)
    {
        uartTx(PRINTF_UART, 'P');
        uartTx(PRINTF_UART, 'R');
        uartTx(PRINTF_UART, 'I');
        uartTx(PRINTF_UART, 'N');
        uartTx(PRINTF_UART, 'T');
        uartTx(PRINTF_UART, 'F');
        uartTx(PRINTF_UART, '_');
        uartTx(PRINTF_UART, 'B');
        uartTx(PRINTF_UART, 'U');
        uartTx(PRINTF_UART, 'F');
        uartTx(PRINTF_UART, 'F');
        uartTx(PRINTF_UART, ' ');
        uartTx(PRINTF_UART, 'T');
        uartTx(PRINTF_UART, 'o');
        uartTx(PRINTF_UART, 'o');
        uartTx(PRINTF_UART, ' ');
        uartTx(PRINTF_UART, 's');
        uartTx(PRINTF_UART, 'm');
        uartTx(PRINTF_UART, 'a');
        uartTx(PRINTF_UART, 'l');
        uartTx(PRINTF_UART, 'l');
        uartTx(PRINTF_UART, '\r');
        uartTx(PRINTF_UART, '\n');
    }
    else
    {
        for (unsigned int i = 0; i < sizeof(tbuff); i++)
        {
            if (!tbuff[i])
                break;
            uartTx(PRINTF_UART, tbuff[i]);
        }
    }
#endif
#if (PRINTF_BUFF_SIZE > 0)
    return ret;
#else
    return 0;
#endif
}

#if (PRINTF_BUFF_SIZE == 0)
void printfOutputHex(__attribute__((unused)) char *marker, __attribute__((unused)) unsigned char *data, __attribute__((unused)) unsigned char dataLen)
{
#else
void printfOutputHex(char *marker, unsigned char *data, unsigned char dataLen)
{

    printf("%s: ", marker);
    for (unsigned int i = 0; i < dataLen; i++)
        printf("%.02X ", data[i]);
    printf("\r\n");
#endif
}

#if (PRINTF_BUFF_SIZE > 0)
void *_sbrk(int32_t incr)
{
#if defined(__NUVO_M032K)
    extern char StackTop;
    char _stack_top = StackTop;
#else
    extern char _stack_top; /* Set by linker.  */
#endif
    static char *heap_end;
    char *prev_heap_end;
    if (heap_end == 0)
    {
        heap_end = &_stack_top;
    }

    prev_heap_end = heap_end;
    heap_end += incr;

    /*printf("_sbrk: Growing memory pool by %d bytes. New span is 0x%X-0x%X (%d bytes)\n",
    	   incr,
    	   (uint32_t)(&end),
    	   (uint32_t)(heap_end),
    	   (uint32_t)(heap_end) - (uint32_t)(&end));
    */
    return (void *)prev_heap_end;
}
#endif