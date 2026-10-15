#include "fft/battle.h"

/* Interpolate an emitter's spawn position from its start to its end value by factor, a fraction of 256:
 * the value of the emitter's animation curve for the frame, or 0 (the start value) when it has none.
 * battle_effect_spawn_emitter_particles and the effect overlays' renderers call it. */
void battle_effect_interpolate_emitter_spawn_position(const void* source, s32 factor, s32* out) {
    const effect_geometry_entry_t* entry = source;

    battle_effect_lerp_and_store(entry->spawn_position_start[0], entry->spawn_position_end[0], factor, out);
    battle_effect_lerp_and_store(entry->spawn_position_start[1], entry->spawn_position_end[1], factor, out + 1);
    battle_effect_lerp_and_store(entry->spawn_position_start[2], entry->spawn_position_end[2], factor, out + 2);
}
