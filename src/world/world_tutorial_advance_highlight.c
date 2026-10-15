#include "fft/world.h"
#include "psx/types.h"

/* Draw the tutorial's highlight ring at its brightness. Until the command is HIDE_HIGHLIGHT the ring
 * stays at full brightness (0x12) and grows by one quad a frame (by the VSync mode) up to its 30 quads.
 * Built with -O0 like the other tutorial handlers. */
void world_tutorial_advance_highlight(void) {
    if (*g_world_tutorial_command_ptr != WORLD_TUTORIAL_CMD_HIDE_HIGHLIGHT) {
        g_world_tutorial_highlight_brightness = 0x12;
        g_world_tutorial_highlight_quad_count += world_gfx_get_vsync_mode_or_one();
        if (g_world_tutorial_highlight_quad_count >= 0x1e) {
            g_world_tutorial_highlight_quad_count = 0x1e;
        }
    }
    world_tutorial_draw_highlight_quads(g_world_tutorial_highlight_brightness);
}
