/* lxstat64 using Linux lstat64 system call.
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

#define __lxstat __redirect___lxstat
#include <sys/stat.h>
#undef __lxstat
#include <fcntl.h>
#include <kernel_stat.h>
#include <sysdep.h>
#include <xstatconv.h>
#include <statx_cp.h>
#include <shlib-compat.h>
#include "ow_syscall_nr.h"

#if LIB_COMPAT(libc, GLIBC_2_1, GLIBC_2_33)

/* Get information about the file NAME in BUF.  */

int
___lxstat64 (int vers, const char *name, struct stat64 *buf)
{
  /* New kABIs which uses generic 64-bit Linux ABI, e.g. aarch64, riscv64.  */
  if (vers == _STAT_VER_KERNEL)
    return INLINE_SYSCALL_CALL (newfstatat, AT_FDCWD, name, buf,
				AT_SYMLINK_NOFOLLOW);

  /* New 32-bit kABIs with only 64-bit time_t support, e.g. arc, riscv32.  */
  if (vers == _STAT_VER_KERNEL)
    {
      struct statx tmp;
      int r = INLINE_SYSCALL_CALL (statx, AT_FDCWD, name,
				   AT_NO_AUTOMOUNT | AT_SYMLINK_NOFOLLOW,
				   STATX_BASIC_STATS, &tmp);
      if (r == 0) {
	      __cp_stat64_statx (buf, &tmp);
        return r;
      }else if(errno != ENOSYS){
        return r;
      }
#if defined __NR_newfstatat
      return INLINE_SYSCALL_CALL (newfstatat, AT_FDCWD, name, buf,
          AT_SYMLINK_NOFOLLOW);
#endif
      return r;
     }

  return INLINE_SYSCALL_ERROR_RETURN_VALUE (EINVAL);
}

#if SHLIB_COMPAT(libc, GLIBC_2_1, GLIBC_2_2)
versioned_symbol (libc, ___lxstat64, __lxstat64, GLIBC_2_2);
strong_alias (___lxstat64, __old__lxstat64)
compat_symbol (libc, __old__lxstat64, __lxstat64, GLIBC_2_1);
#else
strong_alias (___lxstat64, __lxstat64);
#endif

#if XSTAT_IS_XSTAT64
strong_alias (___lxstat64,__lxstat)
#endif

#endif /* LIB_COMPAT  */
