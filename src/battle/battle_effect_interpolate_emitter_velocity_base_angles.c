#include "fft/battle.h"

void battle_effect_interpolate_emitter_velocity_base_angles(
    const effect_geometry_record_t* source, s32 factor, VECTOR* out) {
    const effect_geometry_entry_t* entry = &source->emitter;

    battle_effect_lerp_and_store(
        entry->velocity_base_angle_start.x, entry->velocity_base_angle_end.x, factor, &out->vx);
    battle_effect_lerp_and_store(
        entry->velocity_base_angle_start.y, entry->velocity_base_angle_end.y, factor, &out->vy);
    battle_effect_lerp_and_store(
        entry->velocity_base_angle_start.z, entry->velocity_base_angle_end.z, factor, &out->vz);
}
