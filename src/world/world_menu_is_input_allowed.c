#include "fft/world.h"
#include "psx/types.h"

/* WORLD's version of battle_menu_is_input_allowed. A help box (box type 0x30) in the text or
 * message slot also blocks input and arms a five-frame block. Otherwise input is allowed while the
 * child slot is idle, no close is requested (parameter 3), menu input is not locked, the help is not
 * open and the block has run out. */
s32 world_menu_is_input_allowed(void) {
    if ((world_thread_is_running_asm(NATIVE_SCREEN_THREAD_SLOT_TEXT)
            && (g_world_thread_contexts[NATIVE_SCREEN_THREAD_SLOT_TEXT].function_parameter_1 & 0x70) == 0x30)
        || (world_thread_is_running_asm(NATIVE_SCREEN_THREAD_SLOT_MESSAGE)
            && (g_world_thread_contexts[NATIVE_SCREEN_THREAD_SLOT_MESSAGE].function_parameter_1 & 0x70) == 0x30)) {
        g_world_menu_input_block_frames = 5;
        return 0;
    }
    if (world_thread_is_previous_running() == 0) {
        if (world_thread_get_current_parameter_3() == 0) {
            if (g_world_menu_input_lock_mode == 0) {
                if (g_world_menu_help_open == 0) {
                    if (g_world_menu_input_block_frames == 0) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
