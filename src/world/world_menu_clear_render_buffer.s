/* Zero count bytes of dst: single bytes, then words, then the remaining
 * bytes. The lead count is dst & 3 rather than 4 minus it, so only a
 * buffer at an even address works (QUIRKS.md). BATTLE's
 * battle_menu_clear_render_buffer is byte-identical. Assembly because it
 * adds with the trapping addi and branches with bgez $zero, which the
 * pinned compilers do not emit.
 *
 * WORLD/WORLD.BIN 0x800ff3d4..0x800ff44f (file 0x1f3d4..0x1f44f).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	world_menu_clear_render_buffer
	.ent	world_menu_clear_render_buffer
world_menu_clear_render_buffer:
	andi	$t0, $a0, 0x3
	beq	$t0, $zero, .L800ff408
	nop
.L800ff3e0:
	beq	$a1, $zero, .L800ff448
	nop
	beq	$t0, $zero, .L800ff408
	nop
	sb	$zero, 0($a0)
	addi	$a0, $a0, 1
	addi	$t0, $t0, -1
	addi	$a1, $a1, -1
	bgez	$zero, .L800ff3e0
	nop
.L800ff408:
	andi	$t1, $a1, 0x3
	srl	$a1, $a1, 0x2
.L800ff410:
	beq	$a1, $zero, .L800ff42c
	nop
	sw	$zero, 0($a0)
	addi	$a0, $a0, 4
	addi	$a1, $a1, -1
	bgez	$zero, .L800ff410
	nop
.L800ff42c:
	beq	$t1, $zero, .L800ff448
	nop
	sb	$zero, 0($a0)
	addi	$a0, $a0, 1
	addi	$t1, $t1, -1
	bgez	$zero, .L800ff42c
	nop
.L800ff448:
	jr	$ra
	nop
	.end	world_menu_clear_render_buffer
	.size	world_menu_clear_render_buffer, . - world_menu_clear_render_buffer
