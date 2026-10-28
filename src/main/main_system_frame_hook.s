/* Return at once: every main loop and blocking wait calls it once per frame,
 * entering at the leading nop. It may have been C built another way, but
 * every C spelling of an empty function under the pinned compilers gives
 * `jr ra; nop` without that nop (-O0 adds a frame).
 *
 * SCUS_942.21 0x800449ec..0x800449f7 (file 0x351ec..0x351f7).
 */
	.set	noreorder
	.set	noat
	.text
	.align	2
	.globl	main_system_frame_hook
	.ent	main_system_frame_hook
main_system_frame_hook:
	nop
	jr	$ra
	nop
	.end	main_system_frame_hook
	.size	main_system_frame_hook, . - main_system_frame_hook
