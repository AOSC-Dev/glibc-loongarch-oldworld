#include<sysdeps/unix/sysv/linux/bits/signum-arch.h>

#ifdef LA_OW_COMPACT
#undef __SIGRTMAX
#define __SIGRTMAX 128
#endif
