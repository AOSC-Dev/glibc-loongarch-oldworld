/* Copyright 2016 Loongson Technology Corporation Limited  */

/* Author: Huang Pei huangpei@loongson.cn */

/*
 * ISA: MIPS64R2
 * ABI: N64
 */
/*
algorithm:

	#.	use ld/ldr to access word/partial word in the string

	#.	use (x - 0x0101010101010101) & (~(x | 0x7f7f7f7f7f7f7f7f) != 0 to
	judge if x has zero byte

	#.	use dctz((x - 0x0101010101010101) & (~(x | 0x7f7f7f7f7f7f7f7f) >> 3
		to get the index of first rightmost zero byte in dword x;

	#.	use dctz(x) = 64 - dclz(~x & (x-1));

	#.	use pointer to the last non zero byte  minus pointer to the start
	of the string to get the length of string

*/


#include <sysdep.h>
#include <sys/asm.h>



#define L_ADDIU  addi.d
#define L_ADDU   add.d
#define L_SUBU   sub.d

#define STRLEN	strlen
#define L(x)	x


/* size_t strlen (const char *s1); */

LEAF(STRLEN)

	.align		4


    preld       0, a0, 0

	nor		t4, zero, zero
	lu12i.w		a2, 0x01010
	andi		t5, a0, 0x7

	li		t7, 0x7
	slli.d		t6, t5, 0x3
	andn		t7, a0, t7
	ld.d		a1, t7, 0
	sub.d		t7, zero, t6
	sll.d		t4, t4, t7
    maskeqz     t4, t4, t6
	srl.d		a1, a1, t6
	or		a1, a1, t4


	ori		a2, a2, 0x101
	nor		t1, a1, zero
	li		a4, 8

    preld       0, a0, 32
	bstrins.d	a2, a2, 63, 32
	sub.d		a5, a4, t5
	move		t5, a0

	sub.d		t0, a1, a2
	slli.d		a3, a2, 7
	nor		a3, zero, a3
	nor		t1, a1, a3

	and		t0, t0, t1
    preld       0, a0, 64
	bnez		t0, strlen_count1	/* instead of use bnel with daddu a0, a0, a5 in branch slot */
	L_ADDU		a0, a0, a5

strlen_loop:
	ld.d		a1, a0, 0
	sub.d		t0, a1, a2
	nor		t1, a1, a3
	and		t0, t0, t1

	L_ADDIU		a0, a0, 8
	beqz		t0, strlen_loop
strlen_count:
    addi.d      a0, a0, -8
strlen_count1:
/*	if has d	ctz */
/*	dctz		t1, t0 */
/*
	addi.d		t1, t0, -1
	nor		t2, t0, zero

	and		t1, t2, t1
	li		t3, 64
	clz.d		t1, t1

	sub.w		t1, t3, t1
*/	

	ctz.d		t1, t0

	L_SUBU		v0, a0, t5
	srli.w		t1, t1, 3
	L_ADDU		v0, v0, t1

	jr		ra
END(STRLEN)

#ifndef ANDROID_CHANGES
#ifdef _LIBC
libc_hidden_builtin_def (strlen)
#endif
#endif

