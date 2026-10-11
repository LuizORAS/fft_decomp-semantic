#include "fft/battle.h"

/* Return the event input word the menus read: the buttons pressed this frame
 * (with key repeat) for 0, the buttons held for any other value. */
u32* battle_script_get_controller_input_pointer(s32 use_current) {
    u32* result = &g_battle_script_event_input;
    if (use_current != 0) {
        result = &g_battle_script_event_current_input;
    }
    return result;
}
