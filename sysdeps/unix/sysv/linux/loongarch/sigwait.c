#include<shlib-compat.h>

#undef weak_alias
#define weak_alias(name, aliasname)
#include<sysdeps/unix/sysv/linux/sigwait.c>

versioned_symbol (libc, __sigwait, sigwait, GLIBC_2_0);
#if OTHER_SHLIB_COMPAT(libpthread, GLIBC_2_0, GLIBC_2_34)
compat_symbol (libpthread, __sigwait, sigwait, GLIBC_2_0);
#endif
