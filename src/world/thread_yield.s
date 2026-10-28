/* Save the current thread's s0-s7, k0, k1, gp, sp, fp and ra in its 1 KB
 * slot of g_threads (from offset 0x10), move g_thread_current_id on to the
 * next slot whose word at 0x48 is set, wrapping from 17 to slot 0 (the
 * main loop), call world_script_update_event_input_state and resume that
 * thread's registers. BATTLE's thread_yield is the same with 16 slots and
 * its own hook. Assembly because a context switch saves and restores k0,
 * k1, $gp and $sp.
 *
 * WORLD/WORLD.BIN 0x800fff50..0x80100083 (file 0x1ff50..0x20083).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	thread_yield
	.ent	thread_yield
thread_yield:
	lui	$at, %hi(g_thread_current_id)
	addu	$at, $at, $zero
	lw	$t0, %lo(g_thread_current_id)($at)
	nop
	sll	$t0, $t0, 0xa
	lui	$at, %hi(g_threads)
	addu	$at, $at, $zero
	lw	$t1, %lo(g_threads)($at)
	nop
	addu	$t0, $t0, $t1
	sw	$s0, 16($t0)
	sw	$s1, 20($t0)
	sw	$s2, 24($t0)
	sw	$s3, 28($t0)
	sw	$s4, 32($t0)
	sw	$s5, 36($t0)
	sw	$s6, 40($t0)
	sw	$s7, 44($t0)
	sw	$k0, 48($t0)
	sw	$k1, 52($t0)
	sw	$gp, 56($t0)
	sw	$sp, 60($t0)
	sw	$s8, 64($t0)
	sw	$ra, 68($t0)
.L800fffb0:
	addiu	$t0, $t0, 1024
	lui	$at, %hi(g_thread_current_id)
	addu	$at, $at, $zero
	lw	$t2, %lo(g_thread_current_id)($at)
	nop
	addiu	$t2, $t2, 1
	lui	$at, %hi(g_thread_current_id)
	addu	$at, $at, $zero
	sw	$t2, %lo(g_thread_current_id)($at)
	ori	$t3, $zero, 0x11
	beq	$t2, $t3, .L800ffff8
	nop
	lw	$t3, 72($t0)
	nop
	bne	$zero, $t3, .L80100010
	nop
	j	.L800fffb0
	nop
.L800ffff8:
	lui	$at, %hi(g_threads)
	addu	$at, $at, $zero
	lw	$t0, %lo(g_threads)($at)
	lui	$at, %hi(g_thread_current_id)
	addu	$at, $at, $zero
	sw	$zero, %lo(g_thread_current_id)($at)
.L80100010:
	jal	world_script_update_event_input_state
	nop
	lui	$at, %hi(g_thread_current_id)
	addu	$at, $at, $zero
	lw	$t0, %lo(g_thread_current_id)($at)
	nop
	sll	$t0, $t0, 0xa
	lui	$at, %hi(g_threads)
	addu	$at, $at, $zero
	lw	$t1, %lo(g_threads)($at)
	nop
	addu	$t0, $t0, $t1
	lw	$s0, 16($t0)
	lw	$s1, 20($t0)
	lw	$s2, 24($t0)
	lw	$s3, 28($t0)
	lw	$s4, 32($t0)
	lw	$s5, 36($t0)
	lw	$s6, 40($t0)
	lw	$s7, 44($t0)
	lw	$k0, 48($t0)
	lw	$k1, 52($t0)
	lw	$gp, 56($t0)
	lw	$sp, 60($t0)
	lw	$s8, 64($t0)
	lw	$ra, 68($t0)
	nop
	jr	$ra
	nop
	.end	thread_yield
	.size	thread_yield, . - thread_yield
