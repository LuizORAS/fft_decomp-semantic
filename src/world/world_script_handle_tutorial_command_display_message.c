#include "fft/world.h"

/* Tutorial command: start the text thread on a new message. Operands: two
 * message-id bytes (little-endian, biased by 0xB800) and a third byte stored
 * as the text thread's y offset.
 *
 * -O0, like the neighbouring tutorial handlers. The message id is summed as
 * pointer arithmetic on purpose: written as plain integer addition, GCC
 * 2.6.3's fold re-associates `hi + (lo + 0xB800)` into `(hi + 0xB800) + lo`,
 * and the target adds the bias to the low byte first. */
void world_script_handle_tutorial_command_display_message(void) {
    world_thread_start(WORLD_THREAD_SLOT_TUTORIAL_TEXT, world_text_character_handling_thread);
    world_thread_set_parameters(WORLD_THREAD_SLOT_TUTORIAL_TEXT, 0x99,
        (s32)((u8*)(g_world_script_tutorial_command_ptr[2] * 0x100)
            + (g_world_script_tutorial_command_ptr[1] + 0xB800)),
        0);
    /* Direct slot/member indexing splits this address at -O0. */
    *(s32*)((u8*)g_world_threads + (u32) & ((native_thread_t*)0)[WORLD_THREAD_SLOT_TUTORIAL_TEXT].task_words[3])
        = g_world_script_tutorial_command_ptr[3];
    g_world_script_tutorial_command_ptr += 4;
}
