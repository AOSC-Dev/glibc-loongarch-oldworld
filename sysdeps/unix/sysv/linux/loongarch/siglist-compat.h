#include <siglist-compat-def.h>
#include <sysdeps/unix/sysv/linux/loongarch/bits/ow_nsig.h>

#if SHLIB_COMPAT (libc, GLIBC_2_3_3, GLIBC_2_32)
DEFINE_COMPAT_SIGLIST (__OW_NSIG, GLIBC_2_3_3)
#endif
