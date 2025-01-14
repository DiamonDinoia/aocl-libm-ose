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
#include "libm_special.h"


float FN_PROTOTYPE_REF(atanf)(float fx)
{

  /* Some constants and split constants. */

  static double piby2 = 1.5707963267948966e+00; /* 0x3ff921fb54442d18 */

  double c, v, s, q, z;
  unsigned int xnan,fux,faux;
  unsigned long long ux, aux, xneg;
  double x;
   GET_BITS_SP32(fx, fux);
   faux = fux & ~SIGNBIT_SP32;

    xnan = (faux > PINFBITPATT_SP32);

      if (xnan)
        {
          /* x is NaN */
#ifdef WINDOWS
		return  __amd_handle_errorf("atanf", __amd_atan, fux|0x00400000, _DOMAIN,0, EDOM, fx, 0.0, 1);
#else
          return fx + fx; /* Raise invalid if it's a signalling NaN */
#endif
        }



	  x = fx;
    GET_BITS_DP64(x, ux);
  /* Find properties of argument fx. */


  aux = ux & ~SIGNBIT_DP64;
  xneg = ux & SIGNBIT_DP64;

  v = x;
  if (xneg) v = -x;

  /* Argument reduction to range [-7/16,7/16] */

  if (aux < 0x3ec0000000000000) /* v < 2.0^(-19) */
    {
      /* x is a good approximation to atan(x) */
      if (aux == 0x0000000000000000)
        return fx;
      else
#ifdef WINDOWS
        return fx ; //valf_with_flags(fx, AMD_F_INEXACT);
#else
	return  __amd_handle_errorf("atanf", __amd_atan, fux, _UNDERFLOW, AMD_F_UNDERFLOW|AMD_F_INEXACT, ERANGE, fx, 0.0, 1);

#endif
    }
  else if (aux < 0x3fdc000000000000) /* v < 7./16. */
    {
      x = v;
      c = 0.0;
    }
  else if (aux < 0x3fe6000000000000) /* v < 11./16. */
    {
      x = (2.0*v-1.0)/(2.0+v);
      /* c = arctan(0.5) */
      c = 4.63647609000806093515e-01; /* 0x3fddac670561bb4f */
    }
  else if (aux < 0x3ff3000000000000) /* v < 19./16. */
    {
      x = (v-1.0)/(1.0+v);
      /* c = arctan(1.) */
      c = 7.85398163397448278999e-01; /* 0x3fe921fb54442d18 */
    }
  else if (aux < 0x4003800000000000) /* v < 39./16. */
    {
      x = (v-1.5)/(1.0+1.5*v);
      /* c = arctan(1.5) */
      c = 9.82793723247329054082e-01; /* 0x3fef730bd281f69b */
    }
  else
    {
    if (aux > 0x4190000000000000)
	{ /* abs(x) > 2^26 => arctan(1/x) is
	     insignificant compared to piby2 */
	  if (xneg)
            return (float)-piby2 ; //valf_with_flags((float)-piby2, AMD_F_INEXACT);
	  else
            return (float)piby2;//valf_with_flags((float)piby2, AMD_F_INEXACT);
	}

      x = -1.0/v;
      /* c = arctan(infinity) */
      c = 1.57079632679489655800e+00; /* 0x3ff921fb54442d18 */
    }

  /* Core approximation: Remez(2,2) on [-7/16,7/16] */

  s = x*x;
  q = x*s*
    (0.296528598819239217902158651186e0 +
     (0.192324546402108583211697690500e0 +
       0.470677934286149214138357545549e-2*s)*s)/
    (0.889585796862432286486651434570e0 +
     (0.111072499995399550138837673349e1 +
       0.299309699959659728404442796915e0*s)*s);

  z = c - (q - x);

  if (xneg) z = -z;
  return (float)z;
}


