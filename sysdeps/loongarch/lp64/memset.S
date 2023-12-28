#ifdef _LIBC
#include <sysdep.h>
#include <sys/regdef.h>
#include <sys/asm.h>
#else
#include <sys/asm.h>
#include <sys/regdef.h>
#endif

#ifdef LOONGSON_TEST
#define MEMSET	_memset
#else
#define MEMSET	memset
#endif

//1st var: void *str  $4 a0
//2nd var: int val  $5   a1 
//3rd var: size_t num  $6  a2

LEAF(MEMSET)

memset:
	.align	6

	bstrins.d a1, a1, 15, 8
	add.d	  t7, a0, a2
	bstrins.d a1, a1, 31, 16
	move	  t0, a0
	bstrins.d a1, a1, 63, 32
	srai.d	  t8, a2, 4         	#num/16
	beqz	  t8, less_16bytes	#num<16
	nop
	srai.d	  t8, a2, 8		#num/256
	bnez	  t8, more_256bytes	#num>256
	nop
	srai.d	  t8, a2, 7		#num/128
	beqz	  t8, less_128bytes	#num<128
	nop
	st.d	  a1, a0, 0		#128<num<256
	st.d	  a1, a0, 8
	st.d	  a1, a0, 16
	st.d	  a1, a0, 24
	st.d	  a1, a0, 32
	st.d	  a1, a0, 40
	st.d	  a1, a0, 48
	st.d	  a1, a0, 56
	st.d	  a1, a0, 64
	st.d	  a1, a0, 72
	st.d	  a1, a0, 80
	st.d	  a1, a0, 88
	st.d	  a1, a0, 96
	st.d	  a1, a0, 104
	st.d	  a1, a0, 112
	st.d	  a1, a0, 120
	st.d	  a1, t7, -128
	st.d	  a1, t7, -120
	st.d	  a1, t7, -112
	st.d	  a1, t7, -104
	st.d	  a1, t7, -96
	st.d	  a1, t7, -88
	st.d	  a1, t7, -80
	st.d	  a1, t7, -72
	st.d	  a1, t7, -64
	st.d	  a1, t7, -56
	st.d	  a1, t7, -48
	st.d	  a1, t7, -40
	st.d	  a1, t7, -32
	st.d	  a1, t7, -24
	st.d	  a1, t7, -16
	st.d	  a1, t7, -8

	jr	  ra
	nop

less_128bytes:
	srai.d	  t8, a2, 6		#num/64
	beqz	  t8, less_64bytes
	nop
	st.d	  a1, a0, 0
	st.d	  a1, a0, 8
	st.d	  a1, a0, 16
	st.d	  a1, a0, 24
	st.d	  a1, a0, 32
	st.d	  a1, a0, 40
	st.d	  a1, a0, 48
	st.d	  a1, a0, 56
	st.d	  a1, t7, -64
	st.d	  a1, t7, -56
	st.d	  a1, t7, -48
	st.d	  a1, t7, -40
	st.d	  a1, t7, -32
	st.d	  a1, t7, -24
	st.d	  a1, t7, -16
	st.d	  a1, t7, -8
	
	jr	  ra
	nop

less_64bytes:
	srai.d	  t8, a2, 5		#num/32
	beqz	  t8, less_32bytes
	nop
	st.d	  a1, a0, 0
	st.d	  a1, a0, 8
	st.d	  a1, a0, 16
	st.d	  a1, a0, 24
	st.d	  a1, t7, -32
	st.d	  a1, t7, -24
	st.d	  a1, t7, -16
	st.d	  a1, t7, -8

	jr	  ra
	nop

less_32bytes:
	st.d	  a1, a0, 0
	st.d	  a1, a0, 8
	st.d	  a1, t7, -16
	st.d	  a1, t7, -8
	
	jr	  ra
	nop

less_16bytes:
	srai.d	  t8, a2, 3		#num/8
	beqz	  t8, less_8bytes
	nop
	st.d	  a1, a0, 0
	st.d	  a1, t7, -8
	
	jr	  ra
	nop

less_8bytes:
	srai.d	  t8, a2, 2
	beqz	  t8, less_4bytes
	nop
	st.w	  a1, a0, 0
	st.w	  a1, t7, -4

	jr	  ra
	nop

less_4bytes:
	srai.d	  t8, a2, 1
	beqz	  t8, less_2bytes
	nop
	st.h	  a1, a0, 0
	st.h	  a1, t7, -2

	jr	  ra
	nop

less_2bytes:
	beqz	  a2, less_1bytes
	nop
	st.b	  a1, a0, 0
	
	jr	  ra
	nop

less_1bytes:
	jr	  ra
	nop

more_256bytes:
	srli.d	  a0, a0, 3
	slli.d	  a0, a0, 3
	addi.d	  a0, a0, 0x8
	st.d	  a1, t0, 0
	sub.d	  t2, t0, a0
	add.d	  t2, t2, a2
	addi.d	  t2, t2, -0x80
	srai.d	  t8, t2, 12
	beqz	  t8, loop_less
	nop

loop_more:
	addi.d	  t4, zero, 4
loop_in:
	st.d	  a1, a0, 0
	st.d	  a1, a0, 8
	st.d	  a1, a0, 16
	st.d	  a1, a0, 24
	st.d	  a1, a0, 32
	st.d	  a1, a0, 40
	st.d	  a1, a0, 48
	st.d	  a1, a0, 56
	st.d	  a1, a0, 64
	st.d	  a1, a0, 72
	st.d	  a1, a0, 80
	st.d	  a1, a0, 88
	st.d	  a1, a0, 96
	st.d	  a1, a0, 104
	st.d	  a1, a0, 112
	st.d	  a1, a0, 120
	addi.d	  a0, a0, 128		#1
	st.d	  a1, a0, 0
	st.d	  a1, a0, 8
	st.d	  a1, a0, 16
	st.d	  a1, a0, 24
	st.d	  a1, a0, 32
	st.d	  a1, a0, 40
	st.d	  a1, a0, 48
	st.d	  a1, a0, 56
	st.d	  a1, a0, 64
	st.d	  a1, a0, 72
	st.d	  a1, a0, 80
	st.d	  a1, a0, 88
	st.d	  a1, a0, 96
	st.d	  a1, a0, 104
	st.d	  a1, a0, 112
	st.d	  a1, a0, 120
	addi.d	  a0, a0, 128		#2
	st.d	  a1, a0, 0
	st.d	  a1, a0, 8
	st.d	  a1, a0, 16
	st.d	  a1, a0, 24
	st.d	  a1, a0, 32
	st.d	  a1, a0, 40
	st.d	  a1, a0, 48
	st.d	  a1, a0, 56
	st.d	  a1, a0, 64
	st.d	  a1, a0, 72
	st.d	  a1, a0, 80
	st.d	  a1, a0, 88
	st.d	  a1, a0, 96
	st.d	  a1, a0, 104
	st.d	  a1, a0, 112
	st.d	  a1, a0, 120
	addi.d	  a0, a0, 128		#3
	st.d	  a1, a0, 0
	st.d	  a1, a0, 8
	st.d	  a1, a0, 16
	st.d	  a1, a0, 24
	st.d	  a1, a0, 32
	st.d	  a1, a0, 40
	st.d	  a1, a0, 48
	st.d	  a1, a0, 56
	st.d	  a1, a0, 64
	st.d	  a1, a0, 72
	st.d	  a1, a0, 80
	st.d	  a1, a0, 88
	st.d	  a1, a0, 96
	st.d	  a1, a0, 104
	st.d	  a1, a0, 112
	st.d	  a1, a0, 120
	addi.d	  a0, a0, 128		#4
	st.d	  a1, a0, 0
	st.d	  a1, a0, 8
	st.d	  a1, a0, 16
	st.d	  a1, a0, 24
	st.d	  a1, a0, 32
	st.d	  a1, a0, 40
	st.d	  a1, a0, 48
	st.d	  a1, a0, 56
	st.d	  a1, a0, 64
	st.d	  a1, a0, 72
	st.d	  a1, a0, 80
	st.d	  a1, a0, 88
	st.d	  a1, a0, 96
	st.d	  a1, a0, 104
	st.d	  a1, a0, 112
	st.d	  a1, a0, 120
	addi.d	  a0, a0, 128		#5
	st.d	  a1, a0, 0
	st.d	  a1, a0, 8
	st.d	  a1, a0, 16
	st.d	  a1, a0, 24
	st.d	  a1, a0, 32
	st.d	  a1, a0, 40
	st.d	  a1, a0, 48
	st.d	  a1, a0, 56
	st.d	  a1, a0, 64
	st.d	  a1, a0, 72
	st.d	  a1, a0, 80
	st.d	  a1, a0, 88
	st.d	  a1, a0, 96
	st.d	  a1, a0, 104
	st.d	  a1, a0, 112
	st.d	  a1, a0, 120
	addi.d	  a0, a0, 128		#6
	st.d	  a1, a0, 0
	st.d	  a1, a0, 8
	st.d	  a1, a0, 16
	st.d	  a1, a0, 24
	st.d	  a1, a0, 32
	st.d	  a1, a0, 40
	st.d	  a1, a0, 48
	st.d	  a1, a0, 56
	st.d	  a1, a0, 64
	st.d	  a1, a0, 72
	st.d	  a1, a0, 80
	st.d	  a1, a0, 88
	st.d	  a1, a0, 96
	st.d	  a1, a0, 104
	st.d	  a1, a0, 112
	st.d	  a1, a0, 120
	addi.d	  a0, a0, 128		#7
	st.d	  a1, a0, 0
	st.d	  a1, a0, 8
	st.d	  a1, a0, 16
	st.d	  a1, a0, 24
	st.d	  a1, a0, 32
	st.d	  a1, a0, 40
	st.d	  a1, a0, 48
	st.d	  a1, a0, 56
	st.d	  a1, a0, 64
	st.d	  a1, a0, 72
	st.d	  a1, a0, 80
	st.d	  a1, a0, 88
	st.d	  a1, a0, 96
	st.d	  a1, a0, 104
	st.d	  a1, a0, 112
	st.d	  a1, a0, 120
	addi.d	  a0, a0, 128		#8
	addi.d	  t4, t4, -1
	bnez	  t4, loop_in
	addi.d	  t8, t8, -1
	bnez	  t8, loop_more
	nop
	lu12i.w	  t3, 1
	addi.d	  t3, t3, -1
	and	  t2, t2, t3

loop_less:
	st.d	  a1, a0, 0
	st.d	  a1, a0, 8
	st.d	  a1, a0, 16
	st.d	  a1, a0, 24
	st.d	  a1, a0, 32
	st.d	  a1, a0, 40
	st.d	  a1, a0, 48
	st.d	  a1, a0, 56
	st.d	  a1, a0, 64
	st.d	  a1, a0, 72
	st.d	  a1, a0, 80
	st.d	  a1, a0, 88
	st.d	  a1, a0, 96
	st.d	  a1, a0, 104
	st.d	  a1, a0, 112
	st.d	  a1, a0, 120
	addi.d	a0, a0,  0x80
	addi.d	t2, t2, -0x80
	slt	t8, t2, zero
	beqz	t8, loop_less
	nop
	st.d	  a1, t7, -128
	st.d	  a1, t7, -120
	st.d	  a1, t7, -112
	st.d	  a1, t7, -104
	st.d	  a1, t7, -96
	st.d	  a1, t7, -88
	st.d	  a1, t7, -80
	st.d	  a1, t7, -72
	st.d	  a1, t7, -64
	st.d	  a1, t7, -56
	st.d	  a1, t7, -48
	st.d	  a1, t7, -40
	st.d	  a1, t7, -32
	st.d	  a1, t7, -24
	st.d	  a1, t7, -16
	st.d	  a1, t7, -8

	move	  v0, t0
	
	jr	  ra
	nop

END(MEMSET)

#ifndef ANDROID_CHANGES
#ifdef _LIBC
libc_hidden_builtin_def (memset)
#endif
#endif
