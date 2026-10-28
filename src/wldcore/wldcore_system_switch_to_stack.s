/* Move the stack to the block whose top word a0 points to (every caller
 * passes the scratchpad's last word): store sp in that word and continue
 * just below it, until wldcore_system_restore_previous_stack. Assembly
 * because replacing $sp is a stack-management ABI boundary that C cannot
 * express.
 *
 * WORLD/WLDCORE.BIN 0x80092b04..0x80092b1b (file 0x2bb04..0x2bb1b).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	wldcore_system_switch_to_stack
	.ent	wldcore_system_switch_to_stack
wldcore_system_switch_to_stack:
	addu	$t0, $a0, $zero
	sw	$sp, 0($t0)
	addiu	$t0, $t0, -4
	addu	$sp, $t0, $zero
	jr	$ra
	nop
	.end	wldcore_system_switch_to_stack
	.size	wldcore_system_switch_to_stack, . - wldcore_system_switch_to_stack
