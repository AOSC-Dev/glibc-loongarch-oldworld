#include <errno.h>
#include <sys/stat.h>
#include <kernel_stat.h>
#include <kernel-features.h>
#include <sysdep.h>

#include <string.h>

int
__xstat32_conv (int vers, struct stat64 *kbuf, struct stat *buf)
{
  switch (vers)
    {
    case _STAT_VER_KERNEL:
      {
	/* Convert current kernel version of `struct stat64' to
           `struct stat'.  */
	buf->st_dev = kbuf->st_dev;
#ifdef _HAVE_STAT___PAD1
	buf->__pad1 = 0;
#endif
#ifdef _HAVE_STAT64___ST_INO
# if !__ASSUME_ST_INO_64_BIT
	if (kbuf->st_ino == 0)
	  buf->st_ino = kbuf->__st_ino;
	else
# endif
	  {
	    buf->st_ino = kbuf->st_ino;
	    if (sizeof (buf->st_ino) != sizeof (kbuf->st_ino)
		&& buf->st_ino != kbuf->st_ino)
	      return INLINE_SYSCALL_ERROR_RETURN_VALUE (EOVERFLOW);
	  }
#else
	buf->st_ino = kbuf->st_ino;
	if (sizeof (buf->st_ino) != sizeof (kbuf->st_ino)
	    && buf->st_ino != kbuf->st_ino)
	  return INLINE_SYSCALL_ERROR_RETURN_VALUE (EOVERFLOW);
#endif
	buf->st_mode = kbuf->st_mode;
	buf->st_nlink = kbuf->st_nlink;
	buf->st_uid = kbuf->st_uid;
	buf->st_gid = kbuf->st_gid;
	buf->st_rdev = kbuf->st_rdev;
#ifdef _HAVE_STAT___PAD2
	buf->__pad2 = 0;
#endif
	buf->st_size = kbuf->st_size;
	/* Check for overflow.  */
	if (sizeof (buf->st_size) != sizeof (kbuf->st_size)
	    && buf->st_size != kbuf->st_size)
	  return INLINE_SYSCALL_ERROR_RETURN_VALUE (EOVERFLOW);
	buf->st_blksize = kbuf->st_blksize;
	buf->st_blocks = kbuf->st_blocks;
	/* Check for overflow.  */
	if (sizeof (buf->st_blocks) != sizeof (kbuf->st_blocks)
	    && buf->st_blocks != kbuf->st_blocks)
	  return INLINE_SYSCALL_ERROR_RETURN_VALUE (EOVERFLOW);
#ifdef _HAVE_STAT_NSEC
	buf->st_atim.tv_sec = kbuf->st_atim.tv_sec;
	buf->st_atim.tv_nsec = kbuf->st_atim.tv_nsec;
	buf->st_mtim.tv_sec = kbuf->st_mtim.tv_sec;
	buf->st_mtim.tv_nsec = kbuf->st_mtim.tv_nsec;
	buf->st_ctim.tv_sec = kbuf->st_ctim.tv_sec;
	buf->st_ctim.tv_nsec = kbuf->st_ctim.tv_nsec;
#else
	buf->st_atime = kbuf->st_atime;
	buf->st_mtime = kbuf->st_mtime;
	buf->st_ctime = kbuf->st_ctime;
#endif

#ifdef _HAVE_STAT___UNUSED1
	buf->__glibc_reserved1 = 0;
#endif
#ifdef _HAVE_STAT___UNUSED2
	buf->__glibc_reserved2 = 0;
#endif
#ifdef _HAVE_STAT___UNUSED3
	buf->__glibc_reserved3 = 0;
#endif
#ifdef _HAVE_STAT___UNUSED4
	buf->__glibc_reserved4 = 0;
#endif
#ifdef _HAVE_STAT___UNUSED5
	buf->__glibc_reserved5 = 0;
#endif
      }
      break;

    default:
      return INLINE_SYSCALL_ERROR_RETURN_VALUE (EINVAL);
    }

  return 0;
}
