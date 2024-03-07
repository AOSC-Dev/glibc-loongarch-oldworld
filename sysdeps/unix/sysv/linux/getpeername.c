/* Copyright (C) 2015-2024 Free Software Foundation, Inc.
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

#include <sys/socket.h>
#include <socketcall.h>

int
__getpeername (int fd, __SOCKADDR_ARG addr, socklen_t *len)
{
#ifdef __ASSUME_GETPEERNAME_SYSCALL
  return INLINE_SYSCALL_CALL (getpeername, fd, addr.__sockaddr__, len);
#else
  return SOCKETCALL (getpeername, fd, addr.__sockaddr__, len);
#endif
}
weak_alias (__getpeername, getpeername)
