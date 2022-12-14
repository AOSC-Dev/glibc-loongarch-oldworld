/* llrint().  LoongArch version.
   Copyright (C) 2022 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#define NO_MATH_REDIRECT
#include <math.h>
#include <libm-alias-double.h>

long long int
__llrint (double x)
{
  long long int result;
  asm volatile ("ftint.l.d \t%0, %1" : "=f" (x) : "f" (x));
  asm volatile ("movfr2gr.d \t%0, %1" : "=r" (result) : "f" (x));
  return result;
}
libm_alias_double (__llrint, llrint)
