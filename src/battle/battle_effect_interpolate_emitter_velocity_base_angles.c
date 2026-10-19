#include "fft/battle_effect.h"

/* Same as battle_effect_interpolate_emitter_spawn_position for the base angles of the emitter's
 * velocity. */
void battle_effect_interpolate_emitter_velocity_base_angles(
    const effect_geometry_record_t* source, s32 factor, VECTOR* out) {
    const effect_geometry_entry_t* entry = &source->emitter;

    battle_effect_lerp_fixed8_and_store(
        entry->spawn.velocity_base_angle_start.x, entry->spawn.velocity_base_angle_end.x, factor, &out->vx);
    battle_effect_lerp_fixed8_and_store(
        entry->spawn.velocity_base_angle_start.y, entry->spawn.velocity_base_angle_end.y, factor, &out->vy);
    battle_effect_lerp_fixed8_and_store(
        entry->spawn.velocity_base_angle_start.z, entry->spawn.velocity_base_angle_end.z, factor, &out->vz);
}
