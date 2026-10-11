#include "fft/battle.h"
#include "psx/types.h"

/* Free the eight colour rotation slots (map_texture_animation_state_t) by
 * clearing their active bytes. The 32 texture animation rows of the map are not
 * touched. */
void battle_map_reset_texture_animations(void) {
    s32 offset;

    offset = (MAP_CLUT_CYCLE_SLOT_COUNT - 1) * MAP_CLUT_CYCLE_SLOT_BYTE_COUNT;
    do {
        ((u8*)g_battle_map_texture_animation_states)[offset] = 0;
        offset -= MAP_CLUT_CYCLE_SLOT_BYTE_COUNT;
    } while (offset >= 0);
}
