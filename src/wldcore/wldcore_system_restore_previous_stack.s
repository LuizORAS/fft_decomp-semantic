/* Return to the stack wldcore_system_switch_to_stack left: the sp it stored
 * in the word just above the current one. Assembly because replacing $sp is
 * a stack-management ABI boundary that C cannot express.
 *
 * WORLD/WLDCORE.BIN 0x80092b1c..0x80092b2b (file 0x2bb1c..0x2bb2b).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	wldcore_system_restore_previous_stack
	.ent	wldcore_system_restore_previous_stack
wldcore_system_restore_previous_stack:
	addiu	$sp, $sp, 4
	lw	$sp, 0($sp)
	jr	$ra
	nop
	.end	wldcore_system_restore_previous_stack
	.size	wldcore_system_restore_previous_stack, . - wldcore_system_restore_previous_stack
