// We neet statx_cp for oldworld compaitibility.
// So include it back and override the oerride in wordsize-64

#include <sysdeps/unix/sysv/linux/statx_cp.c>
