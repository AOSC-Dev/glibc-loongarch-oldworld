/* Copyright (C) 2011-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Chris Metcalf <cmetcalf@tilera.com>, 2011.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* Ignore prototype to avoid error if we alias __lxstat and __lxstat64. */
#define __lxstat64 __lxstat64_disable

#include <errno.h>
#include <stddef.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <kernel_stat.h>

#include <sysdep.h>
#include <sys/syscall.h>

#include <xstatconv.h>
#include <statx_cp.h>

/* Get information about the file NAME in BUF.  */
int
__lxstat (int vers, const char *name, struct stat *buf)
{
  if (vers != _STAT_VER_KERNEL)
    {
      errno = EINVAL;
      return -1;
    }
  struct stat64 kst64;
  struct statx tmp;
  int rc = INLINE_SYSCALL (statx, 5, AT_FDCWD, name,
                           AT_NO_AUTOMOUNT | AT_SYMLINK_NOFOLLOW,
                           STATX_BASIC_STATS, &tmp);

  if (rc < 0)
    return rc;

  __cp_stat64_statx (&kst64, &tmp);
  return __xstat32_conv (vers, &kst64, buf);
}

hidden_def (__lxstat)
weak_alias (__lxstat, _lxstat);
#if XSTAT_IS_XSTAT64
#undef __lxstat64
strong_alias (__lxstat, __lxstat64);
hidden_ver (__lxstat, __lxstat64)
#endif
