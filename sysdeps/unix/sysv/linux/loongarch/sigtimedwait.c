#include<shlib-compat.h>

#undef weak_alias
#define weak_alias(name, aliasname)
#include<sysdeps/unix/sysv/linux/sigtimedwait.c>

versioned_symbol (libc, __sigtimedwait, sigtimedwait, GLIBC_2_1);
