/*
 * Copyright (C) 2008-2020 Advanced Micro Devices, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "libm_amd.h"
#include "libm_util_amd.h"
#include <stdio.h>

double  FN_PROTOTYPE_REF(nan)(const char *tagp)
{


    /* Check for input range */
    UT64 checkbits;
    U64 val=0;
    S64 num;
    checkbits.u64  =QNANBITPATT_DP64; 
    if(tagp == NULL)
    {
      return checkbits.f64;
    }

    switch(*tagp)
    {
    case '0': /* base 8 */
                tagp++; 
                if( *tagp == 'x' || *tagp == 'X')
                {
                    /* base 16 */
                    tagp++;
                    while(*tagp != '\0')
                    {
                        
                        if(*tagp >= 'A' && *tagp <= 'F' )
                        {
                            num = *tagp - 'A' + 10;
                        }
                        else
                        if(*tagp >= 'a' && *tagp <= 'f' )
                        {                          
                            num = *tagp - 'a' + 10;  
                        }
                        else
                        {
                            num = *tagp - '0'; 
                        }                        

                        if( (num < 0 || num > 15))
                        {
                            val = QNANBITPATT_DP64;
                            break;
                        }
                        val = (val << 4)  |  num; 
                        tagp++;
                    }
                }
                else
                {
                    /* base 8 */
                    while(*tagp != '\0')
                    {
                        num = *tagp - '0';
                        if( num < 0 || num > 7)
                        {
                            val = QNANBITPATT_DP64;
                            break;
                        }
                        val = (val << 3)  |  num; 
                        tagp++;
                    }
                }
		break;
    default:
                while(*tagp != '\0')
                {
                    val = val*10;
                    num = *tagp - '0';
                    if( num < 0 || num > 9)
                    {
                        val = QNANBITPATT_DP64;
                        break;
                    }
                    val = val + num; 
                    tagp++;
                }
            
    }

   if((val & ~NINFBITPATT_DP64) == 0)
	val = QNANBITPATT_DP64;
	 
    checkbits.u64 = (val | QNANBITPATT_DP64) & ~SIGNBIT_DP64;
    return checkbits.f64  ;
}



