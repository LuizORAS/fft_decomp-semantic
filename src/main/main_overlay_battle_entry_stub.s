/* BATTLE.BIN's entry point as main_overlay_call_battle_entrypoint calls it:
 * 4 KB of zero words, which run as nops, then a return, so the call does
 * nothing. The original keeps these 4 KB between the C start-up and the game
 * code for no reason the code shows; no C function holds 1024 nops.
 *
 * SCUS_942.21 0x80010ba8..0x80011baf (file 0x13a8..0x23af).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	main_overlay_battle_entry_stub
	.ent	main_overlay_battle_entry_stub
main_overlay_battle_entry_stub:
	.rept	1024
	nop
	.endr
	jr	$ra
	nop
	.end	main_overlay_battle_entry_stub
	.size	main_overlay_battle_entry_stub, . - main_overlay_battle_entry_stub
