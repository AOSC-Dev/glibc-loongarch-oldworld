#include <shlib-compat.h>

#if LIB_COMPAT(libc, GLIBC_2_27, GLIBC_2_36)
#include <sysdep.h>
#undef HAVE_INTERNAL_BRK_ADDR_SYMBOL
#define HAVE_INTERNAL_BRK_ADDR_SYMBOL 1
#endif

#include <sysdeps/unix/sysv/linux/brk.c>
