/***************************************************************************//**
* \file retarget.c
* \version 1.20
*
* \brief
* 'Retarget' layer for target-dependent low level function.
*
********************************************************************************
* \copyright
* Copyright 2016-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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
*******************************************************************************/

#include <stdio.h>
#include "stdio_user.h"

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef IO_STDOUT_ENABLE

#if defined(__ARMCC_VERSION) /* ARM-MDK */
    /***************************************************************************
    * Function Name: fputc
    ***************************************************************************/
    __attribute__((weak)) int fputc(int ch, FILE *f)
    {
        (void)f;
    #if (STDOUT_CR_LF != 0)
        if ((char)ch == '\n')
        {
            STDIO_PutChar('\r');
        }
    #endif /* STDOUT_CR_LF != 0 */
        STDIO_PutChar(ch);
        return (ch);
    }
#elif defined (__ICCARM__) /* IAR */
    #include <yfuns.h>

    /***************************************************************************
    * Function Name: __write
    ***************************************************************************/
    __weak size_t __write(int handle, const unsigned char * buffer, size_t size)
    {
        size_t nChars = 0;
        /* This template only writes to "standard out", for all other file
        * handles it returns failure. */
        if (handle != _LLIO_STDOUT)
        {
            return (_LLIO_ERROR);
        }
        if (buffer != NULL)
        {
            for (/* Empty */; nChars < size; ++nChars)
            {
            #if (STDOUT_CR_LF != 0)
                if(*buffer == '\n')
                {
                    STDIO_PutChar('\r');
                }
            #endif /* STDOUT_CR_LF != 0 */
                STDIO_PutChar(*buffer);
                ++buffer;
            }
        }
        return (nChars);
    }
#else /* (__GNUC__)  GCC */    
    /* Add an explicit reference to the floating point printf library to allow
    the usage of floating point conversion specifier. */
    __asm (".global _printf_float");
    /***************************************************************************
    * Function Name: _write
    ***************************************************************************/
    __attribute__((weak)) int _write (int fd, const char *ptr, int len)
    {
        int nChars = 0;
        (void)fd;
        if (ptr != NULL)
        {
            for (/* Empty */; nChars < len; ++nChars)
            {
            #if (STDOUT_CR_LF != 0)
                if(*ptr == '\n')
                {
                    STDIO_PutChar('\r');
                }
            #endif /* STDOUT_CR_LF != 0 */
                STDIO_PutChar((uint32_t)*ptr);
                ++ptr;
            }
        }
        return (nChars);
    }
#endif

#endif /* IO_STDOUT_ENABLE */

#if defined(IO_STDIN_ENABLE)

#if defined(__ARMCC_VERSION) /* ARM-MDK */
    /***************************************************************************
    * Function Name: fgetc
    ***************************************************************************/
    __attribute__((weak)) int fgetc(FILE *f) 
    {
        (void)f;
        return (STDIO_GetChar());
    }
#elif defined (__ICCARM__) /* IAR */
    __weak size_t __read(int handle, unsigned char * buffer, size_t size)
    {
        /* This template only reads from "standard in", for all other file
        handles it returns failure. */
        if ((handle != _LLIO_STDIN) || (buffer == NULL))
        {
            return (_LLIO_ERROR);
        }
        else
        {
            *buffer = STDIO_GetChar();
            return (1);
        }
    }
#else /* (__GNUC__)  GCC */
    /* Add an explicit reference to the floating point scanf library to allow
    the usage of floating point conversion specifier. */
    __asm (".global _scanf_float");
    __attribute__((weak)) int _read (int fd, char *ptr, int len)
    {
        int nChars = 0;
        (void)fd;
        if (ptr != NULL)
        {
            for(/* Empty */;nChars < len;++ptr)
            {
                *ptr = (char)STDIO_GetChar();
                ++nChars;
                if((*ptr == '\n') || (*ptr == '\r'))
                {
                    break;
                }
            }
        }
        return (nChars);
    }
#endif 

#endif /* IO_STDIN_ENABLE */

#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */

