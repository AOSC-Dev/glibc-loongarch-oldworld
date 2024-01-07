/* fxstat using old-style Unix fstat system call.
   Copyright (C) 1991-2018 Free Software Foundation, Inc.
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
   <http://www.gnu.org/licenses/>.  */

/* Ho hum, since xstat == xstat64 we must get rid of the prototype or gcc
   will complain since they don't strictly match.  */
#define __fxstat64 __fxstat64_disable

#include <errno.h>
#include <stddef.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <kernel_stat.h>

#include <sysdep.h>
#include <sys/syscall.h>

#include <xstatconv.h>
#include <statx_cp.h>

/* Get information about the file FD in BUF.  */
int
__fxstat (int vers, int fd, struct stat *buf)
{
  if (vers != _STAT_VER_KERNEL && vers != _STAT_VER_LINUX)
    {
      __set_errno (EINVAL);
      return -1;
    }

  struct stat64 kst64;
  struct statx tmp;

  INTERNAL_SYSCALL_DECL (err);
  unsigned long sys_result = INTERNAL_SYSCALL (
      statx, err, 5, fd, "", AT_EMPTY_PATH, STATX_BASIC_STATS, &tmp);
  if (__builtin_expect (INTERNAL_SYSCALL_ERROR_P (sys_result, err), 0))
    {
      if (INTERNAL_SYSCALL_ERRNO (sys_result, err) == ENOSYS)
        {
          return INLINE_SYSCALL (fstat, 2, fd, buf);
        }
      else
        {
          __set_errno (INTERNAL_SYSCALL_ERRNO (sys_result, err));
          return -1;
        }
    }

  __cp_stat64_statx (&kst64, &tmp);
  return __xstat32_conv (vers, &kst64, buf);
}

hidden_def (__fxstat)
weak_alias (__fxstat, _fxstat);
#undef __fxstat64
strong_alias (__fxstat, __fxstat64);
hidden_ver (__fxstat, __fxstat64)
