#include "fft/battle.h"

/* Same as battle_effect_interpolate_emitter_spawn_position for the base angles of the emitter's
 * velocity. */
void battle_effect_interpolate_emitter_velocity_base_angles(const void* source, s32 factor, s32* out) {
    const effect_geometry_entry_t* entry = source;

    battle_effect_lerp_and_store(entry->velocity_base_angle_start[0], entry->velocity_base_angle_end[0], factor, out);
    battle_effect_lerp_and_store(
        entry->velocity_base_angle_start[1], entry->velocity_base_angle_end[1], factor, out + 1);
    battle_effect_lerp_and_store(
        entry->velocity_base_angle_start[2], entry->velocity_base_angle_end[2], factor, out + 2);
}
