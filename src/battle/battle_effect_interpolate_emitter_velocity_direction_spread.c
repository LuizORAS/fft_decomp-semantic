#include "fft/battle.h"

void battle_effect_interpolate_emitter_velocity_direction_spread(
    const effect_geometry_record_t* source, s32 factor, VECTOR* out) {
    const effect_geometry_entry_t* entry = &source->emitter;

    battle_effect_lerp_and_store(
        entry->spawn.velocity_spread_start.x, entry->spawn.velocity_spread_end.x, factor, &out->vx);
    battle_effect_lerp_and_store(
        entry->spawn.velocity_spread_start.y, entry->spawn.velocity_spread_end.y, factor, &out->vy);
    battle_effect_lerp_and_store(
        entry->spawn.velocity_spread_start.z, entry->spawn.velocity_spread_end.z, factor, &out->vz);
}
