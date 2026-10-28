/* Zero bytes bytes of buffer: single bytes, then words, then the
 * remaining bytes. The lead count is buffer & 3 rather than 4 minus it, so
 * only a buffer at an even address works (QUIRKS.md). WORLD's
 * world_menu_clear_render_buffer is byte-identical. Assembly because it
 * adds with the trapping addi and branches with bgez $zero, which the
 * pinned compilers do not emit.
 *
 * BATTLE.BIN 0x8014bed8..0x8014bf53 (file 0xe4ed8..0xe4f53).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	battle_menu_clear_render_buffer
	.ent	battle_menu_clear_render_buffer
battle_menu_clear_render_buffer:
	andi	$t0, $a0, 0x3
	beq	$t0, $zero, .L8014bf0c
	nop
.L8014bee4:
	beq	$a1, $zero, .L8014bf4c
	nop
	beq	$t0, $zero, .L8014bf0c
	nop
	sb	$zero, 0($a0)
	addi	$a0, $a0, 1
	addi	$t0, $t0, -1
	addi	$a1, $a1, -1
	bgez	$zero, .L8014bee4
	nop
.L8014bf0c:
	andi	$t1, $a1, 0x3
	srl	$a1, $a1, 0x2
.L8014bf14:
	beq	$a1, $zero, .L8014bf30
	nop
	sw	$zero, 0($a0)
	addi	$a0, $a0, 4
	addi	$a1, $a1, -1
	bgez	$zero, .L8014bf14
	nop
.L8014bf30:
	beq	$t1, $zero, .L8014bf4c
	nop
	sb	$zero, 0($a0)
	addi	$a0, $a0, 1
	addi	$t1, $t1, -1
	bgez	$zero, .L8014bf30
	nop
.L8014bf4c:
	jr	$ra
	nop
	.end	battle_menu_clear_render_buffer
	.size	battle_menu_clear_render_buffer, . - battle_menu_clear_render_buffer
