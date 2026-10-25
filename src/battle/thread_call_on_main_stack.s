/* Call g_thread_call_target on the main loop's stack, with the caller's
 * arguments and result passing through in registers. From a thread
 * (g_thread_current_id nonzero) it moves sp 16 bytes below the sp saved
 * in slot 0 of g_threads for the call and back after it; from the main
 * loop it calls the target where it is. The return address waits in
 * g_thread_call_saved_ra. Assembly because it replaces $sp and keeps ra
 * in a global across the call.
 *
 * BATTLE.BIN 0x8014ceb4..0x8014cf57 (file 0xe5eb4..0xe5f57).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	thread_call_on_main_stack
	.ent	thread_call_on_main_stack
thread_call_on_main_stack:
	lui	$at, %hi(g_thread_call_saved_ra)
	addu	$at, $at, $zero
	sw	$ra, %lo(g_thread_call_saved_ra)($at)
	lui	$at, %hi(g_thread_current_id)
	addu	$at, $at, $zero
	lw	$t0, %lo(g_thread_current_id)($at)
	nop
	beq	$t0, $zero, .L8014cf28
	nop
	lui	$at, %hi(g_threads)
	addu	$at, $at, $zero
	lw	$t0, %lo(g_threads)($at)
	nop
	lw	$t0, 60($t0)
	nop
	addi	$t0, $t0, -16
	sw	$sp, 4($t0)
	sw	$ra, 8($t0)
	addu	$sp, $t0, $zero
	lui	$at, %hi(g_thread_call_target)
	addu	$at, $at, $zero
	lw	$t0, %lo(g_thread_call_target)($at)
	nop
	jalr	$t0
	nop
	lw	$ra, 8($sp)
	lw	$sp, 4($sp)
	jr	$ra
	nop
.L8014cf28:
	lui	$at, %hi(g_thread_call_target)
	addu	$at, $at, $zero
	lw	$t0, %lo(g_thread_call_target)($at)
	nop
	jalr	$t0
	nop
	lui	$at, %hi(g_thread_call_saved_ra)
	addu	$at, $at, $zero
	lw	$ra, %lo(g_thread_call_saved_ra)($at)
	nop
	jr	$ra
	nop
	.end	thread_call_on_main_stack
	.size	thread_call_on_main_stack, . - thread_call_on_main_stack
