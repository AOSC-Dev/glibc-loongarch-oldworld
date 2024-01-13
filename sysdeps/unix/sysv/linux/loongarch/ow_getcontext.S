#include <shlib-compat.h>
#include "ow_ucontext-macros.h"

#if SHLIB_COMPAT (libc, GLIBC_2_1, GLIBC_2_36)
#undef versioned_symbol
#define versioned_symbol(a, b, c, d)
#define __getcontext __ow___getcontext
#include "getcontext.S"

#undef __getcontext
compat_symbol (libc, __ow___getcontext, getcontext, GLIBC_2_1);

#endif
