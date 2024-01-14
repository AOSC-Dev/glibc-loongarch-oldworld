#include <sysdeps/unix/sysv/linux/fstat64.c>

// In glibc 2.39, fstat64 changed its implementation to use fstat() in 551101e
// However, on loongarch64, it still calls __fstatat64_time64, unchanged.
// So the magic will happen there, no magic is needed here.
