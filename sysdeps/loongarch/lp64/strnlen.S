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

#include <sys/asm.h>
#include <sys/regdef.h>



#define L_ADDIU  addi.d
#define L_ADDU   add.d
#define L_SUBU   sub.d

#define STRNLEN	__strnlen
#define L(x)	x
/* rd <- if rc then ra else rb 
    will destroy t6
*/

#define CONDITIONSEL(rd,rc,ra,rb)\
        masknez t6, rb, rc;\
        maskeqz rd, ra, rc;\
        or      rd, rd, t6



/* size_t strnlen (const char *s1,size_t maxlen); */

LEAF(STRNLEN)

	.align		4


    beqz        a1, L(_out)
	preld		0, a0, 0
    move        t2, a1

	nor		t4, zero, zero
	lu12i.w		a2, 0x01010
	andi		t5, a0, 0x7

	li		t7, 0x7
    li      t3, 8
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
    sub.d       t3, t3, t5
	move		t5, a0

	sub.d		t0, a1, a2
	slli.d		a3, a2, 7
    sltu        t6, t3, t2
	nor		a3, zero, a3
	nor		t1, a1, a3
    
    xori        t6, t6, 0x1

	and		t0, t0, t1
    or          t6, t6, t0
    preld       0, a0, 64
	bnez		t6, L(_count1)	/* instead of use bnel with daddu a0, a0, a5 in branch slot */
	L_ADDU		a0, a0, a5

L(_loop):
	ld.d		a1, a0, 0
	L_ADDIU		a0, a0, 8
	sub.d		t0, a1, a2
    sub.d       t3, a0, t5
	nor		t1, a1, a3
    sltu        t6, t3, t2
	and		t0, t0, t1

    xori        t6, t6, 0x1
    or          t6, t6, t0
	beqz		t6, L(_loop)
L(_count):
    addi.d      a0, a0, -8
L(_count1):

	ctz.d		t1, t0

	L_SUBU		v0, a0, t5
	srli.w		t1, t1, 3
	L_ADDU		v0, v0, t1
    sltu        t7, t2, v0
    CONDITIONSEL(v0,t7,t2,v0)
	jr		ra
L(_out):
    move v0, zero
    jr  ra
END(STRNLEN)
#ifndef ANDROID_CHANGES
#ifdef _LIBC
weak_alias (__strnlen, strnlen)
libc_hidden_def (strnlen)
libc_hidden_def (__strnlen)
#endif
#endif
