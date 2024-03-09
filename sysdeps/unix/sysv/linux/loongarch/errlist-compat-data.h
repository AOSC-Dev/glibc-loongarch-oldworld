#include <errlist-compat.h>

// See sysdeps/unix/sysv/linux/loongarch/Versions of ow glibc 2.28
// They copied from mips, so we also copy the corresponding
// errlist-compat statements from mips
#if SHLIB_COMPAT (libc, GLIBC_2_2, GLIBC_2_32)
DEFINE_COMPAT_ERRLIST (1134, GLIBC_2_1)
#endif
