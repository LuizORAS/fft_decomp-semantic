#include "fft/battle_effect.h"
#include "fft/battle_map.h"
#include "fft/battle_unit.h"
#include "fft/map.h"
#include "psx/libgte.h"

/*
 * Secondary effect function 0x09 (Splash) in g_battle_effect_secondary_handlers.
 * It shares the fall dust
 * handler's shape; each executing frame first lifts group 7 to the water
 * surface of the target's tile, 12 units per height and depth step above the
 * target's screen z. Returns 1 to keep animating.
 */
s32 battle_effect_splash_secondary_handler(void) {
    battle_effect_secondary_data_t* effect;
    map_tile_t* tile;
    s32 unused[6]; /* Reserves the target's 0x48-byte frame. */
    battle_effect_gravity_t gravity;
    s32 result;
    s16 finish_timer;

    /* Preserve the target's unsigned phase-dispatch comparison. */
    switch (g_battle_effect_current_secondary->phase) {
    case BATTLE_EFFECT_SECONDARY_PHASE_INITIALIZING:
        result = 1;
        g_battle_effect_current_secondary->active_count = 0;
        g_battle_effect_current_secondary->timer = 0;
        g_battle_effect_current_secondary->finish_timer = 0;
        g_battle_effect_current_secondary->phase = BATTLE_EFFECT_SECONDARY_PHASE_EXECUTING;
        break;
    case BATTLE_EFFECT_SECONDARY_PHASE_EXECUTING:
        tile = battle_map_get_unit_tile_from_world_coords(
            g_battle_effect_current_secondary->target.record.data.unit.runtime_id);
        g_battle_effect_particle_groups[BATTLE_EFFECT_PARTICLE_GROUP_SPLASH].position.y
            = -((tile->height + tile->depth_half_height.depth) * 12)
            - battle_unit_get_world_coords_by_runtime_id(
                g_battle_effect_current_secondary->target.record.data.unit.runtime_id)
                  ->vy;
        battle_effect_start_group(BATTLE_EFFECT_PARTICLE_GROUP_SPLASH, 11);
        /* fallthrough */
    case BATTLE_EFFECT_SECONDARY_PHASE_FINALIZING:
        battle_effect_save_inertia_threshold();
        battle_effect_set_inertia_threshold(BATTLE_EFFECT_SECONDARY_INERTIA_THRESHOLD);
        gravity.z = 0;
        gravity.x = 0;
        gravity.y = ONE;
        battle_effect_save_gravity();
        battle_effect_set_gravity(&gravity);
        battle_effect_update_active_slots(BATTLE_EFFECT_PARTICLE_GROUP_SPLASH);
        battle_effect_restore_inertia_threshold();
        battle_effect_restore_gravity();
        effect = g_battle_effect_current_secondary;
        finish_timer = effect->finish_timer;
        result = finish_timer != 1;
        effect->timer = effect->timer + 1;
        if (effect->active_count == 0
            && effect->timer > g_battle_effect_particle_groups[BATTLE_EFFECT_PARTICLE_GROUP_SPLASH].spawn_start_frame) {
            effect->finish_timer = finish_timer + 1;
        }
        break;
    }
    return result;
}
