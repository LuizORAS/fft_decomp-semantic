/* Return a * b / c through the 64-bit product, by 64 rounds of shift and
 * subtract. On the negative-result path a quotient above 32 bits sets
 * g_camera_cross_product_overflow; its negations are wrong (QUIRKS.md).
 * WORLD's world_util_mul_div_s64 is the same routine. Assembly because it
 * keeps s0-s7 in the scratchpad instead of a stack frame and adds with the
 * trapping addi, which the pinned compilers do not emit.
 *
 * BATTLE.BIN 0x8014ccb8..0x8014ce07 (file 0xe5cb8..0xe5e07).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	battle_util_mul_div_s64
	.ent	battle_util_mul_div_s64
battle_util_mul_div_s64:
	mult	$a0, $a1
	lui	$t0, 0x1f80
	sw	$s0, 0($t0)
	sw	$s1, 4($t0)
	sw	$s2, 8($t0)
	sw	$s3, 12($t0)
	sw	$s4, 16($t0)
	sw	$s5, 20($t0)
	sw	$s6, 24($t0)
	sw	$s7, 28($t0)
	addi	$t8, $zero, -1
	mflo	$t0
	mfhi	$t1
	addu	$t2, $zero, $zero
	addu	$t3, $zero, $zero
	addu	$t4, $zero, $zero
	srl	$s2, $t1, 0x1f
	beq	$s2, $zero, .L8014cd1c
	nop
	xor	$t0, $t0, $t8
	xor	$t1, $t1, $t8
	addiu	$t0, $t0, 1
	bne	$t0, $zero, .L8014cd1c
	nop
	addiu	$t2, $t2, 1
.L8014cd1c:
	srl	$s3, $a2, 0x1f
	beq	$s3, $zero, .L8014cd30
	nop
	xor	$a2, $a2, $t8
	addiu	$t0, $t0, 1
.L8014cd30:
	addiu	$s7, $zero, 64
.L8014cd34:
	srl	$s4, $t0, 0x1f
	srl	$s5, $t1, 0x1f
	sll	$t2, $t2, 0x1
	beq	$s5, $zero, .L8014cd4c
	nop
	ori	$t2, $t2, 0x1
.L8014cd4c:
	sll	$t1, $t1, 0x1
	beq	$s4, $zero, .L8014cd5c
	nop
	ori	$t1, $t1, 0x1
.L8014cd5c:
	sll	$t0, $t0, 0x1
	addiu	$s6, $zero, 0
	slt	$at, $t2, $a2
	bne	$at, $zero, .L8014cd78
	nop
	subu	$t2, $t2, $a2
	addiu	$s6, $zero, 1
.L8014cd78:
	srl	$s4, $t3, 0x1f
	sll	$t4, $t4, 0x1
	beq	$s4, $zero, .L8014cd8c
	nop
	ori	$t4, $t4, 0x1
.L8014cd8c:
	sll	$t3, $t3, 0x1
	or	$t3, $t3, $s6
	addi	$s7, $s7, -1
	bne	$s7, $zero, .L8014cd34
	nop
	xor	$s2, $s3, $s2
	beq	$s2, $zero, .L8014cdd8
	nop
	beq	$t4, $zero, .L8014cdd0
	nop
	addiu	$t4, $t4, 1
	beq	$t4, $zero, .L8014cdd0
	nop
	addi	$t4, $zero, 1
	lui	$at, %hi(g_camera_cross_product_overflow)
	addu	$at, $at, $zero
	sw	$t4, %lo(g_camera_cross_product_overflow)($at)
.L8014cdd0:
	xor	$t3, $t3, $t8
	addiu	$t3, $t3, 1
.L8014cdd8:
	addu	$v0, $zero, $t3
	lui	$t0, 0x1f80
	lw	$s0, 0($t0)
	lw	$s1, 4($t0)
	lw	$s2, 8($t0)
	lw	$s3, 12($t0)
	lw	$s4, 16($t0)
	lw	$s5, 20($t0)
	lw	$s6, 24($t0)
	lw	$s7, 28($t0)
	jr	$ra
	nop
	.end	battle_util_mul_div_s64
	.size	battle_util_mul_div_s64, . - battle_util_mul_div_s64
