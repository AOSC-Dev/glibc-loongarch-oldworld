#include <shlib-compat.h>

#if LIB_COMPAT(libc, GLIBC_2_27, GLIBC_2_36)
# include <kernel-features.h>
# ifdef __ASSUME_STATX
#  undef __ASSUME_STATX
# endif
// Hack: force use statx for fstatat64 even __NR_newfstatat is available
# define STAT_HAS_TIME32
# include "ow_syscall_nr.h"
#endif

#include <sysdeps/unix/sysv/linux/fstatat64.c>
