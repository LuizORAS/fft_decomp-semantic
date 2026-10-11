#include "fft/battle.h"
#include "psx/types.h"

/* Starts an ability's effect outside an action, on a unit, on a tile or
 * on a unit that is also its caster. */
void battle_effect_start_new(s32 ability_id, s32 target_mode, battle_effect_start_target_data_t target) {
    battle_effect_secondary_init_t effect_data;

    effect_data.target_count = 1;
    effect_data.targets[BATTLE_EFFECT_CASTER_ENTRY].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    effect_data.targets[BATTLE_EFFECT_CASTER_ENTRY].id.unit.misc_id = 0;

    if (target_mode == BATTLE_EFFECT_START_ON_UNIT) {
        effect_data.targets[0].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
        effect_data.targets[0].id.unit.misc_id = target.misc_id;
        effect_data.targets[0].id.unit.result_animation = BATTLE_EFFECT_HIT_NORMAL;
    } else if (target_mode == BATTLE_EFFECT_START_ON_TILE) {
        effect_data.targets[0].target_type = BATTLE_EFFECT_TARGET_TYPE_TILE;
        effect_data.targets[0].id.tile_x = target.tile->vx;
        effect_data.targets[0].tile_y = target.tile->vz;
        effect_data.targets[0].map_z = target.tile->vy;
    } else {
        effect_data.targets[0].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
        effect_data.targets[0].id.unit.misc_id = target.misc_id;
        effect_data.targets[0].id.unit.result_animation = BATTLE_EFFECT_HIT_NORMAL;
        effect_data.targets[BATTLE_EFFECT_CASTER_ENTRY].id.unit.misc_id = target.misc_id;
    }

    effect_data.spectator_count = 0;
    battle_effect_set_ability_animation(0, ability_id, &effect_data);
}
