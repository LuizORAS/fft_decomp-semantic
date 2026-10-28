/* LIBCARD's jump template, which _patch_card copies into the BIOS exception
 * handler: load psyq_card_interrupt_tail's address and jump there. The
 * original keeps these five words between StopCARD2 and
 * psyq_card_interrupt_tail. Code that runs from another place, with its
 * target split over lui and addiu, has no C form; from source the link moves
 * the target with the function.
 *
 * SCUS_942.21 0x800288c4..0x800288d7 (file 0x190c4..0x190d7).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	g_psyq_card_interrupt_jump_template
	.ent	g_psyq_card_interrupt_jump_template
g_psyq_card_interrupt_jump_template:
	lui	$v0, %hi(psyq_card_interrupt_tail)
	addiu	$v0, $v0, %lo(psyq_card_interrupt_tail)
	jr	$v0
	nop
	nop
	.end	g_psyq_card_interrupt_jump_template
	.size	g_psyq_card_interrupt_jump_template, . - g_psyq_card_interrupt_jump_template
