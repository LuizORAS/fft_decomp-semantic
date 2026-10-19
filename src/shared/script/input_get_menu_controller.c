#include "fft/script.h"

/* Return the 32-bit event input value the menus read: the buttons pressed this frame
 * (with key repeat) for 0, the buttons held for any other value. */
u32* input_get_menu_controller(s32 use_current) {
    u32* result = &g_script_event_input;
    if (use_current != 0) {
        result = &g_script_event_current_input;
    }
    return result;
}
