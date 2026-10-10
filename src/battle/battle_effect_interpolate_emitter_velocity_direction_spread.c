#include "fft/battle.h"

void battle_effect_interpolate_emitter_velocity_direction_spread(const void* source, s32 factor, s32* out) {
    const effect_geometry_entry_t* entry = source;

    battle_effect_lerp_and_store(entry->velocity_spread_start[0], entry->velocity_spread_end[0], factor, out);
    battle_effect_lerp_and_store(entry->velocity_spread_start[1], entry->velocity_spread_end[1], factor, out + 1);
    battle_effect_lerp_and_store(entry->velocity_spread_start[2], entry->velocity_spread_end[2], factor, out + 2);
}
