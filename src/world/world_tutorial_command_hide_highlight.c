#include "fft/world.h"

/* Tutorial command HIDE_HIGHLIGHT: fade the highlight ring out, its brightness counting down by the
 * VSync mode, and move to the next command when it reaches zero. */
void world_tutorial_command_hide_highlight(void) {
    g_world_tutorial_highlight_brightness -= world_gfx_get_vsync_mode_or_one();
    if (g_world_tutorial_highlight_brightness <= 0) {
        g_world_tutorial_highlight_brightness = 0;
        g_world_tutorial_command_ptr++;
    }
}
