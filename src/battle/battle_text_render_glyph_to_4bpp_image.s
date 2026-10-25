/* Draw a 10x14 glyph of 2-bit pixels (four to a byte, high bits first)
 * from glyph_bitmap into the 4bpp image at origin's x and y with its
 * stride: each nonzero pixel becomes its value plus palette_offset, and
 * only rows from text variable $27 up to $28
 * (g_battle_text_substitution_value_27 and _28: the first row and the row
 * limit) are drawn. WORLD's world_text_blit_font_glyph_to_4bpp is the same
 * routine. Assembly because it
 * adds with the trapping addi and branches with bgez $zero, which the
 * pinned compilers do not emit, and keeps its registers in the
 * scratchpad instead of a stack frame.
 *
 * BATTLE.BIN 0x8014bd88..0x8014bed7 (file 0xe4d88..0xe4ed7).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	battle_text_render_glyph_to_4bpp_image
	.ent	battle_text_render_glyph_to_4bpp_image
battle_text_render_glyph_to_4bpp_image:
	lui	$t0, 0x1f80
	sw	$s0, 0($t0)
	sw	$s1, 4($t0)
	sw	$s2, 8($t0)
	sw	$s3, 12($t0)
	sw	$s4, 16($t0)
	sw	$s5, 20($t0)
	sw	$s6, 24($t0)
	sw	$s7, 28($t0)
	sw	$v0, 32($t0)
	sw	$v1, 36($t0)
	lui	$at, %hi(g_battle_text_substitution_value_27)
	addu	$at, $at, $zero
	lw	$v0, %lo(g_battle_text_substitution_value_27)($at)
	lui	$at, %hi(g_battle_text_substitution_value_28)
	addu	$at, $at, $zero
	lw	$v1, %lo(g_battle_text_substitution_value_28)($at)
	lhu	$t8, 0($a2)
	lhu	$t9, 2($a2)
	lhu	$s1, 8($a2)
	nop
	srl	$s1, $s1, 0x1
	multu	$s1, $t9
	addi	$s0, $zero, 10
	addi	$s7, $zero, 14
	addi	$t6, $zero, 4
	lbu	$t7, 0($a0)
	mflo	$t5
	add	$t5, $t5, $a1
	addu	$t2, $zero, $zero
.L8014be00:
	slt	$at, $t2, $v1
	beq	$at, $zero, .L8014be94
	nop
	addu	$t3, $zero, $zero
.L8014be10:
	andi	$s3, $t7, 0xc0
	srl	$s3, $s3, 0x6
	beq	$s3, $zero, .L8014be6c
	nop
	add	$s3, $s3, $a3
	add	$t4, $t3, $t8
	andi	$t4, $t4, 0x1
	add	$s4, $t3, $t8
	srl	$s4, $s4, 0x1
	add	$s4, $s4, $t5
	slt	$at, $t2, $v0
	bne	$at, $zero, .L8014be6c
	nop
	lbu	$s5, 0($s4)
	bne	$t4, $zero, .L8014be5c
	nop
	andi	$s5, $s5, 0xf0
	bgez	$zero, .L8014be64
	nop
.L8014be5c:
	andi	$s5, $s5, 0xf
	sll	$s3, $s3, 0x4
.L8014be64:
	or	$s5, $s5, $s3
	sb	$s5, 0($s4)
.L8014be6c:
	sll	$t7, $t7, 0x2
	addi	$t6, $t6, -1
	bne	$t6, $zero, .L8014be88
	nop
	addi	$t6, $zero, 4
	addi	$a0, $a0, 1
	lbu	$t7, 0($a0)
.L8014be88:
	addi	$t3, $t3, 1
	bne	$t3, $s0, .L8014be10
	nop
.L8014be94:
	add	$t5, $t5, $s1
	addi	$t2, $t2, 1
	bne	$t2, $s7, .L8014be00
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
	lw	$v0, 32($t0)
	lw	$v1, 36($t0)
	jr	$ra
	nop
	.end	battle_text_render_glyph_to_4bpp_image
	.size	battle_text_render_glyph_to_4bpp_image, . - battle_text_render_glyph_to_4bpp_image
