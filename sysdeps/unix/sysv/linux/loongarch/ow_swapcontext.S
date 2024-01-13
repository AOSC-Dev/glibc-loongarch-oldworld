#include <shlib-compat.h>
#include "ow_ucontext-macros.h"

#if SHLIB_COMPAT (libc, GLIBC_2_1, GLIBC_2_36)
#undef versioned_symbol
#define versioned_symbol(a, b, c, d)
#define __swapcontext __ow___swapcontext
#include "swapcontext.S"

#undef __swapcontext
compat_symbol (libc, __ow___swapcontext, swapcontext, GLIBC_2_1);

#endif
