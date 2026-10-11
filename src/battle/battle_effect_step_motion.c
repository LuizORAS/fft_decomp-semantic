#include "fft/battle.h"
#include "psx/types.h"

/* Advance an effect slot's motion state by one frame. */
void battle_effect_step_motion(battle_effect_motion_t* motion) {
    s32 unused[4]; /* The target reserves an unused 16-byte frame. */

    motion->position.x += motion->velocity.x;
    motion->position.y += motion->velocity.y;
    motion->position.z += motion->velocity.z;
    motion->velocity.x
        = ((motion->inertia - g_battle_effect_inertia_threshold) * motion->velocity.x + (motion->acceleration.x << 12))
            / motion->inertia
        + ((motion->weight * g_battle_effect_gravity.x) >> 12);
    motion->velocity.y
        = ((motion->inertia - g_battle_effect_inertia_threshold) * motion->velocity.y + (motion->acceleration.y << 12))
            / motion->inertia
        + ((motion->weight * g_battle_effect_gravity.y) >> 12);
    motion->velocity.z
        = ((motion->inertia - g_battle_effect_inertia_threshold) * motion->velocity.z + (motion->acceleration.z << 12))
            / motion->inertia
        + ((motion->weight * g_battle_effect_gravity.z) >> 12);
    motion->acceleration.x += motion->acceleration_step.x;
    motion->acceleration.y += motion->acceleration_step.y;
    motion->acceleration.z += motion->acceleration_step.z;
}
