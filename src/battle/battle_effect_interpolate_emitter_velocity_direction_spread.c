#include "fft/battle_effect.h"

/* Same as battle_effect_interpolate_emitter_spawn_position for the spread of the velocity's direction.
 * Only the effect overlays' renderers call it: battle_effect_spawn_emitter_particles works the same three
 * values out with battle_effect_lerp_fixed8. */
void battle_effect_interpolate_emitter_velocity_direction_spread(
    const effect_geometry_record_t* source, s32 factor, VECTOR* out) {
    const effect_geometry_entry_t* entry = &source->emitter;

    battle_effect_lerp_fixed8_and_store(
        entry->spawn.velocity_spread_start.x, entry->spawn.velocity_spread_end.x, factor, &out->vx);
    battle_effect_lerp_fixed8_and_store(
        entry->spawn.velocity_spread_start.y, entry->spawn.velocity_spread_end.y, factor, &out->vy);
    battle_effect_lerp_fixed8_and_store(
        entry->spawn.velocity_spread_start.z, entry->spawn.velocity_spread_end.z, factor, &out->vz);
}
