#include "fft/battle.h"
#include "psx/types.h"

/* Advance an effect slot's motion state by one frame. */
void battle_effect_step_motion(battle_effect_motion_t* motion) {
    s32 unused[4]; /* The target reserves an unused 16-byte frame. */

    motion->position[0] += motion->velocity[0];
    motion->position[1] += motion->velocity[1];
    motion->position[2] += motion->velocity[2];
    motion->velocity[0] = ((motion->inertia - g_battle_effect_inertia_threshold) * motion->velocity[0]
                              + (motion->acceleration[0] << 12))
            / motion->inertia
        + ((motion->weight * g_battle_effect_gravity.x) >> 12);
    motion->velocity[1] = ((motion->inertia - g_battle_effect_inertia_threshold) * motion->velocity[1]
                              + (motion->acceleration[1] << 12))
            / motion->inertia
        + ((motion->weight * g_battle_effect_gravity.y) >> 12);
    motion->velocity[2] = ((motion->inertia - g_battle_effect_inertia_threshold) * motion->velocity[2]
                              + (motion->acceleration[2] << 12))
            / motion->inertia
        + ((motion->weight * g_battle_effect_gravity.z) >> 12);
    motion->acceleration[0] += motion->acceleration_step[0];
    motion->acceleration[1] += motion->acceleration_step[1];
    motion->acceleration[2] += motion->acceleration_step[2];
}
