/* Return a * b / c through the 64-bit product, by 64 rounds of shift and
 * subtract. On the negative-result path a quotient above 32 bits sets
 * g_world_camera_cross_product_overflow; its negations are wrong
 * (QUIRKS.md). BATTLE's battle_util_mul_div_s64 is the same routine.
 * Assembly because it keeps s0-s7 in the scratchpad instead of a stack
 * frame and adds with the trapping addi, which the pinned compilers do not
 * emit.
 *
 * WORLD/WORLD.BIN 0x80100188..0x801002d7 (file 0x20188..0x202d7).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	world_util_mul_div_s64
	.ent	world_util_mul_div_s64
world_util_mul_div_s64:
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
	beq	$s2, $zero, .L801001ec
	nop
	xor	$t0, $t0, $t8
	xor	$t1, $t1, $t8
	addiu	$t0, $t0, 1
	bne	$t0, $zero, .L801001ec
	nop
	addiu	$t2, $t2, 1
.L801001ec:
	srl	$s3, $a2, 0x1f
	beq	$s3, $zero, .L80100200
	nop
	xor	$a2, $a2, $t8
	addiu	$t0, $t0, 1
.L80100200:
	addiu	$s7, $zero, 64
.L80100204:
	srl	$s4, $t0, 0x1f
	srl	$s5, $t1, 0x1f
	sll	$t2, $t2, 0x1
	beq	$s5, $zero, .L8010021c
	nop
	ori	$t2, $t2, 0x1
.L8010021c:
	sll	$t1, $t1, 0x1
	beq	$s4, $zero, .L8010022c
	nop
	ori	$t1, $t1, 0x1
.L8010022c:
	sll	$t0, $t0, 0x1
	addiu	$s6, $zero, 0
	slt	$at, $t2, $a2
	bne	$at, $zero, .L80100248
	nop
	subu	$t2, $t2, $a2
	addiu	$s6, $zero, 1
.L80100248:
	srl	$s4, $t3, 0x1f
	sll	$t4, $t4, 0x1
	beq	$s4, $zero, .L8010025c
	nop
	ori	$t4, $t4, 0x1
.L8010025c:
	sll	$t3, $t3, 0x1
	or	$t3, $t3, $s6
	addi	$s7, $s7, -1
	bne	$s7, $zero, .L80100204
	nop
	xor	$s2, $s3, $s2
	beq	$s2, $zero, .L801002a8
	nop
	beq	$t4, $zero, .L801002a0
	nop
	addiu	$t4, $t4, 1
	beq	$t4, $zero, .L801002a0
	nop
	addi	$t4, $zero, 1
	lui	$at, %hi(g_world_camera_cross_product_overflow)
	addu	$at, $at, $zero
	sw	$t4, %lo(g_world_camera_cross_product_overflow)($at)
.L801002a0:
	xor	$t3, $t3, $t8
	addiu	$t3, $t3, 1
.L801002a8:
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
	.end	world_util_mul_div_s64
	.size	world_util_mul_div_s64, . - world_util_mul_div_s64
