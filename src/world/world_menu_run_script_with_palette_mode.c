#include "fft/world.h"

/* Type debt (QUIRKS.md): output is a display script (byte stream), but the
 * callers declare their scripts as u8[], s16[] or world_menu_window_command_t. */
void world_menu_run_script_with_palette_mode(void* output, s32 input, s32 mode) {
    s32 value;

    value = input;
    if (mode != 0) {
        value = 0;
    }
    world_menu_set_palette_mode(mode);
    world_menu_run_display_script(output, value);
}
