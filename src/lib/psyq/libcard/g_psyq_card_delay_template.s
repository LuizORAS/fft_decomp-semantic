/* LIBCARD's call template, which _patch_card2 swaps with five instructions
 * of the BIOS's pad-clear routine: the routine then calls this template's own
 * place, where the five instructions it displaced run and fall through into
 * psyq_card_short_delay. Code that runs from another place, with its own
 * address split over lui and addiu, has no C form; from source the link
 * moves that address with it.
 *
 * SCUS_942.21 0x800289a4..0x800289b7 (file 0x191a4..0x191b7).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	g_psyq_card_delay_template
	.ent	g_psyq_card_delay_template
g_psyq_card_delay_template:
	lui	$t0, %hi(g_psyq_card_delay_template)
	addiu	$t0, $t0, %lo(g_psyq_card_delay_template)
	jalr	$t0
	nop
	nop
	.end	g_psyq_card_delay_template
	.size	g_psyq_card_delay_template, . - g_psyq_card_delay_template
