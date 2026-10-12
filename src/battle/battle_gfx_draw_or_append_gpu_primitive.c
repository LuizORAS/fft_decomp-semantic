#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_gfx.h"

/*
 * Submit a primitive for the active rendering mode.
 *
 * ETC full-screen graphics draw immediately. Normal battle rendering links
 * the primitive into the current ordering-table entry.
 */
void battle_gfx_draw_or_append_gpu_primitive(void* primitive) {
    if (g_battle_etc_graphics_enabled != 0) {
        DrawPrim(primitive);
        return;
    }
    addPrim(g_battle_gfx_current_otag_entry, primitive);
}
