/* Copy the 4bpp pixels of source_rect's rect of source (its x, y, width,
 * height and stride) into image at position's x and y with position's
 * stride, skipping zero (transparent) pixels, and a whole zero word at a
 * time, and adding position->color's nibble to each copied pixel: BATTLE's
 * text_blit_glyph without its x test. Assembly because it
 * adds with the trapping addi and branches with bgez $zero, which the
 * pinned compilers do not emit, and keeps its registers in the
 * scratchpad instead of a stack frame.
 *
 * WORLD/WORLD.BIN 0x800feff0..0x800ff283 (file 0x1eff0..0x1f283).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	world_text_blit_glyph
	.ent	world_text_blit_glyph
world_text_blit_glyph:
	lui	$t0, 0x1f80
	sw	$s0, 0($t0)
	sw	$s1, 4($t0)
	sw	$s2, 8($t0)
	sw	$s3, 12($t0)
	sw	$s4, 16($t0)
	sw	$s5, 20($t0)
	sw	$s6, 24($t0)
	sw	$s7, 28($t0)
	sw	$v0, 80($t0)
	sw	$v1, 84($t0)
	sw	$s8, 88($t0)
	addi	$s0, $zero, 15
	lhu	$s7, 0($a2)
	lhu	$t1, 2($a2)
	lhu	$s2, 8($a2)
	lhu	$t8, 0($a3)
	lhu	$t9, 2($a3)
	lhu	$s4, 8($a3)
	lw	$s8, 12($a3)
	sll	$v1, $s2, 0x2
	sw	$v1, 48($t0)
	srl	$s2, $s2, 0x1
	multu	$s2, $t1
	lhu	$v0, 4($a2)
	lhu	$v1, 6($a2)
	sw	$v0, 72($t0)
	sw	$v1, 76($t0)
	andi	$v0, $s7, 0x1
	sll	$v0, $v0, 0x2
	srl	$s7, $s7, 0x1
	mflo	$s2
	sll	$v1, $s4, 0x2
	sw	$v1, 52($t0)
	srl	$s4, $s4, 0x1
	multu	$s4, $t9
	add	$s2, $s2, $s7
	add	$s2, $s2, $a0
	andi	$v1, $s2, 0x3
	sll	$v1, $v1, 0x3
	add	$v1, $v1, $v0
	sw	$v1, 56($t0)
	add	$t6, $zero, $v1
	sllv	$t4, $s0, $v1
	addi	$v0, $zero, -4
	and	$s2, $s2, $v0
	sw	$s2, 32($t0)
	lw	$s3, 0($s2)
	mflo	$s4
	andi	$v0, $t8, 0x1
	sll	$v0, $v0, 0x2
	srl	$t8, $t8, 0x1
	add	$s4, $s4, $t8
	add	$s4, $s4, $a1
	andi	$v1, $s4, 0x3
	sll	$v1, $v1, 0x3
	add	$v1, $v1, $v0
	sw	$v1, 60($t0)
	add	$t7, $zero, $v1
	sllv	$t5, $s0, $v1
	addi	$v0, $zero, -4
	and	$s4, $s4, $v0
	sw	$s4, 36($t0)
	lw	$s5, 0($s4)
	lw	$t2, 76($t0)
.L800ff0f4:
	lw	$t3, 72($t0)
.L800ff0f8:
	bne	$s3, $zero, .L800ff138
	nop
	slti	$at, $t3, 8
	bne	$at, $zero, .L800ff138
	nop
	ori	$at, $zero, 0x8
	beq	$t3, $at, .L800ff1cc
	nop
	sw	$s5, 0($s4)
	addi	$s2, $s2, 4
	addi	$s4, $s4, 4
	lw	$s5, 0($s4)
	lw	$s3, 0($s2)
	addi	$t3, $t3, -7
	bgez	$zero, .L800ff1c0
	nop
.L800ff138:
	and	$v0, $s3, $t4
	beq	$v0, $zero, .L800ff17c
	nop
	addu	$v0, $v0, $s8
	and	$v0, $v0, $t4
	nor	$v1, $zero, $t5
	and	$s5, $s5, $v1
	slt	$at, $t6, $t7
	bne	$at, $zero, .L800ff170
	nop
	sub	$s1, $t6, $t7
	srlv	$v0, $v0, $s1
	bgez	$zero, .L800ff178
	nop
.L800ff170:
	sub	$s1, $t7, $t6
	sllv	$v0, $v0, $s1
.L800ff178:
	or	$s5, $s5, $v0
.L800ff17c:
	addi	$t6, $t6, 4
	sll	$t4, $t4, 0x4
	bne	$t4, $zero, .L800ff19c
	nop
	addi	$s2, $s2, 4
	addi	$t4, $zero, 15
	addi	$t6, $zero, 0
	lw	$s3, 0($s2)
.L800ff19c:
	addi	$t7, $t7, 4
	sll	$t5, $t5, 0x4
	bne	$t5, $zero, .L800ff1c0
	nop
	sw	$s5, 0($s4)
	addi	$s4, $s4, 4
	addi	$t5, $zero, 15
	addi	$t7, $zero, 0
	lw	$s5, 0($s4)
.L800ff1c0:
	addi	$t3, $t3, -1
	bne	$t3, $zero, .L800ff0f8
	nop
.L800ff1cc:
	sw	$s5, 0($s4)
	lw	$s4, 36($t0)
	lw	$v0, 52($t0)
	lw	$v1, 60($t0)
	nop
	add	$v0, $v0, $v1
	srl	$v1, $v0, 0x5
	sll	$v1, $v1, 0x2
	add	$s4, $s4, $v1
	sw	$s4, 36($t0)
	andi	$v0, $v0, 0x1f
	sw	$v0, 60($t0)
	sllv	$t5, $s0, $v0
	add	$t7, $zero, $v0
	lw	$s5, 0($s4)
	lw	$s2, 32($t0)
	lw	$v0, 48($t0)
	lw	$v1, 56($t0)
	nop
	add	$v0, $v0, $v1
	srl	$v1, $v0, 0x5
	sll	$v1, $v1, 0x2
	add	$s2, $s2, $v1
	sw	$s2, 32($t0)
	andi	$v0, $v0, 0x1f
	sw	$v0, 56($t0)
	sllv	$t4, $s0, $v0
	add	$t6, $zero, $v0
	lw	$s3, 0($s2)
	addi	$t2, $t2, -1
	bne	$t2, $zero, .L800ff0f4
	nop
	lui	$t0, 0x1f80
	lw	$s0, 0($t0)
	lw	$s1, 4($t0)
	lw	$s2, 8($t0)
	lw	$s3, 12($t0)
	lw	$s4, 16($t0)
	lw	$s5, 20($t0)
	lw	$s6, 24($t0)
	lw	$s7, 28($t0)
	lw	$v0, 80($t0)
	lw	$v1, 84($t0)
	lw	$s8, 88($t0)
	jr	$ra
	nop
	.end	world_text_blit_glyph
	.size	world_text_blit_glyph, . - world_text_blit_glyph
