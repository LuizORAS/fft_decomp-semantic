#include "fft/battle_effect.h"

void battle_effect_interpolate_emitter_spawn_spread(const effect_geometry_record_t* source, s32 factor, VECTOR* out) {
    const effect_geometry_entry_t* entry = &source->emitter;

    battle_lerp_fixed8_and_store(entry->spawn.spawn_spread_start.x, entry->spawn.spawn_spread_end.x, factor, &out->vx);
    battle_lerp_fixed8_and_store(entry->spawn.spawn_spread_start.y, entry->spawn.spawn_spread_end.y, factor, &out->vy);
    battle_lerp_fixed8_and_store(entry->spawn.spawn_spread_start.z, entry->spawn.spawn_spread_end.z, factor, &out->vz);
}
