/* getpid - get the pid.  Linux/Loongarch version.
   Copyright (C) 2015-2018 Free Software Foundation, Inc.
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

#include <unistd.h>

#ifdef SHARED
# include <dl-vdso.h>
# include <errno.h>

static pid_t
__getpid_syscall (void)
{
  return INLINE_SYSCALL (getpid, 0);
}

# ifndef __getpid_type
#  define __getpid_type __getpid
# endif

# undef INIT_ARCH
# define INIT_ARCH() PREPARE_VERSION_KNOWN (linux26, LINUX_2_6)
libc_ifunc_hidden (__getpid_type, __getpid, (_dl_vdso_vsym ("__vdso_getpid", &linux26) ?: &__getpid_syscall))
libc_hidden_def (__getpid)

#else

# include <sysdep.h>
# include <errno.h>

pid_t
__getpid (void)
{
  return INLINE_SYSCALL (getpid, 0);
}
libc_hidden_def (__getpid);

#endif
weak_alias (__getpid, getpid)
libc_hidden_weak (getpid)
