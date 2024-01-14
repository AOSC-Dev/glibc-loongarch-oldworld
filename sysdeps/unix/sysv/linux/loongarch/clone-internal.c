#include <shlib-compat.h>

#if LIB_COMPAT(libc, GLIBC_2_27, GLIBC_2_36)
#include <sysdep.h>
#undef HAVE_CLONE3_WRAPPER
#endif

#include <sysdeps/unix/sysv/linux/clone-internal.c>
