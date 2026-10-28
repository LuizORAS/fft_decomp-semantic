/* Return bits 12-43 of the 64-bit a * b - c * d, setting
 * g_world_camera_cross_product_overflow when bits 44-63 of that difference
 * are neither all zero nor all one (QUIRKS.md). BATTLE's
 * battle_util_cross_product_q12 is the same routine. Assembly because it
 * sets the flag with the trapping addi from $zero, which the pinned
 * compilers do not emit.
 *
 * WORLD/WORLD.BIN 0x801002d8..0x80100347 (file 0x202d8..0x20347).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	world_util_cross_product_q12
	.ent	world_util_cross_product_q12
world_util_cross_product_q12:
	mult	$a0, $a1
	mflo	$a0
	mfhi	$a1
	nop
	nop
	mult	$a2, $a3
	mflo	$a2
	mfhi	$a3
	sltu	$t0, $a0, $a2
	subu	$v0, $a0, $a2
	subu	$v1, $a1, $a3
	subu	$v1, $v1, $t0
	addu	$t0, $zero, $v1
	sra	$t0, $t0, 0xc
	beq	$t0, $zero, .L80100334
	nop
	addiu	$t0, $t0, 1
	beq	$t0, $zero, .L80100334
	nop
	addi	$t0, $zero, 1
	lui	$at, %hi(g_world_camera_cross_product_overflow)
	addu	$at, $at, $zero
	sw	$t0, %lo(g_world_camera_cross_product_overflow)($at)
.L80100334:
	sll	$v1, $v1, 0x14
	srl	$v0, $v0, 0xc
	or	$v0, $v0, $v1
	jr	$ra
	nop
	.end	world_util_cross_product_q12
	.size	world_util_cross_product_q12, . - world_util_cross_product_q12
