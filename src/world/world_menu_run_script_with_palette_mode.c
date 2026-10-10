#include "fft/world.h"

/* Run a menu display script in palette mode `mode`; a nonzero mode (the
 * dimmed palette) also withholds `input` from the script. */
void world_menu_run_script_with_palette_mode(void* output, s32 input, s32 mode) {
    s32 value;

    value = input;
    if (mode != 0) {
        value = 0;
    }
    world_menu_set_palette_mode(mode);
    world_menu_run_display_script(output, value);
}
