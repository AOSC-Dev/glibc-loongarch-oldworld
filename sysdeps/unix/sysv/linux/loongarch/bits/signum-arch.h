#include<sysdeps/unix/sysv/linux/bits/signum-arch.h>

#ifdef LA_OW_COMPACT
#include <sysdeps/unix/sysv/linux/loongarch/bits/ow_nsig.h>
#undef __SIGRTMAX
#define __SIGRTMAX __OW_NSIG
#endif
