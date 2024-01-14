/* xstat64 using Linux stat64 system call.
   Copyright (C) 1991-2023 Free Software Foundation, Inc.
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

#define __xstat __redirect___xstat
#include <sys/stat.h>
#undef __xstat
#include <fcntl.h>
#include <kernel_stat.h>
#include <sysdep.h>
#include <xstatconv.h>
#include <statx_cp.h>
#include <shlib-compat.h>
#include "ow_syscall_nr.h"

#if LIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_33)

/* Get information about the file NAME in BUF.  */

int
___xstat64 (int vers, const char *name, struct stat64 *buf)
{
  /* New 32-bit kABIs with only 64-bit time_t support, e.g. arc, riscv32.  */
  if (vers == _STAT_VER_KERNEL)
    {
      struct statx tmp;
      int r = INLINE_SYSCALL_CALL (statx, AT_FDCWD, name, AT_NO_AUTOMOUNT,
				   STATX_BASIC_STATS, &tmp);
      if (r == 0) {
        __cp_stat64_statx (buf, &tmp);
        return r;
      }else if(errno != ENOSYS){
        return r;
      }
#if defined __NR_newfstatat
      return INLINE_SYSCALL_CALL (newfstatat, AT_FDCWD, name, buf, 0);
#endif
      return r;
     }

  return INLINE_SYSCALL_ERROR_RETURN_VALUE (EINVAL);
}

#if XSTAT_IS_XSTAT64
strong_alias (___xstat64, __xstat)
#endif

#if SHLIB_COMPAT(libc, GLIBC_2_1, GLIBC_2_2)
versioned_symbol (libc, ___xstat64, __xstat64, GLIBC_2_2);
strong_alias (___xstat64, __old__xstat64)
compat_symbol (libc, __old__xstat64, __xstat64, GLIBC_2_1);
#else
strong_alias (___xstat64, __xstat64)
#endif


#endif /* LIB_COMPAT  */
