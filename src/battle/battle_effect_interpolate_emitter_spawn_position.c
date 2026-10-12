#include "fft/battle_effect.h"

void battle_effect_interpolate_emitter_spawn_position(const effect_geometry_record_t* source, s32 factor, VECTOR* out) {
    const effect_geometry_entry_t* entry = &source->emitter;

    battle_effect_lerp_and_store(
        entry->spawn.spawn_position_start.x, entry->spawn.spawn_position_end.x, factor, &out->vx);
    battle_effect_lerp_and_store(
        entry->spawn.spawn_position_start.y, entry->spawn.spawn_position_end.y, factor, &out->vy);
    battle_effect_lerp_and_store(
        entry->spawn.spawn_position_start.z, entry->spawn.spawn_position_end.z, factor, &out->vz);
}
