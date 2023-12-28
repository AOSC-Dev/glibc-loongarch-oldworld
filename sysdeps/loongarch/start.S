#define __ASSEMBLY__ 1
#include <entry.h>
#include <sys/asm.h>

/* The entry point's job is to call __libc_start_main.  Per the ABI,
   a0 contains the address of a function to be passed to atexit.
   __libc_start_main wants this in a5.  */

/*
int
__libc_start_main (int (*main) (int, char **, char **),
			    int argc,
			    char **argv,
			    __typeof (main) init,
			    void (*fini) (void),
			    void (*rtld_fini) (void),
			    void *stack_end);
*/

ENTRY (ENTRY_POINT)
	/* Terminate call stack by noting ra is undefined.  Use a dummy
	   .cfi_label to force starting the FDE.  */
	.cfi_label .Ldummy
	cfi_undefined (1)
	or	a5, a0, zero /* rtld_fini */

	/* 这个main必须要走GOT表拿到。因为main不一定是local的。
	   比如googletest就把main定义在动态库里了。 */
	la.got	a0, t0, main
#ifdef __loongarch64
	ld.d	a1, sp, 0
	addi.d	a2, sp, SZREG
#elif defined __loongarch32
	ld.w	a1, sp, 0
	addi.w	a2, sp, SZREG
#endif
	/* Adjust $sp for 16-aligned */
	srli.d  sp, sp, 4
	slli.d  sp, sp, 4

	la.got	a3, t0,  __libc_csu_init
	la.got	a4, t0,  __libc_csu_fini
	or	a6, sp, zero /* stack_end. */

	la.got	ra, t0, __libc_start_main
	jirl	ra, ra, 0

	la.got	ra, t0, abort
	jirl	ra, ra, 0
END (ENTRY_POINT)

