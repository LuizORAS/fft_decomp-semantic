#include "fft/battle.h"
#include "psx/types.h"

/* Stop a colour rotation by the handle battle_map_start_texture_animation
 * returned (slot index + 1). Returns 1 for a handle 1-8, else 0.
 *
 * Retail clears the active byte at handle * 10, not (handle - 1) * 10: it stops
 * the slot after the handle's own, and handle 8 clears the byte that follows the
 * eight slots, the first of g_battle_gfx_cycled_deployed_palettes. The byte view
 * keeps that addressing. */
s32 battle_map_stop_texture_animation(s32 handle) {
    if (handle >= 1 && handle <= 8) {
        ((u8*)g_battle_map_texture_animation_states)[handle * MAP_TEXTURE_ANIMATION_STATE_BYTES] = 0;
        return 1;
    }
    return 0;
}
