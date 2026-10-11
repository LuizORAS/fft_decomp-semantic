#include "fft/battle.h"
#include "psx/types.h"

/* Starts an ability's effect outside an action, on a unit, on a tile or
 * on a unit that is also its caster. */
void battle_effect_start_new(s32 ability_id, s32 target_mode, battle_effect_start_target_data_t target) {
    battle_effect_secondary_init_t effect_data;

    effect_data.target_count = 1;
    effect_data.caster.fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    effect_data.caster.fields.caster_id = 0;

    if (target_mode == BATTLE_EFFECT_START_ON_UNIT) {
        effect_data.target[0].fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
        effect_data.target[0].fields.target_id = target.misc_id;
        effect_data.target[0].fields.result_animation = BATTLE_EFFECT_HIT_NORMAL;
    } else if (target_mode == BATTLE_EFFECT_START_ON_TILE) {
        effect_data.target[0].tile.target_type = BATTLE_EFFECT_TARGET_TYPE_TILE;
        effect_data.target[0].tile.id.tile_x = target.tile->vx;
        effect_data.target[0].tile.tile_y = target.tile->vz;
        effect_data.target[0].tile.map_z = target.tile->vy;
    } else {
        effect_data.target[0].fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
        effect_data.target[0].fields.target_id = target.misc_id;
        effect_data.target[0].fields.result_animation = BATTLE_EFFECT_HIT_NORMAL;
        effect_data.caster.fields.caster_id = target.misc_id;
    }

    effect_data.spectator_count = 0;
    battle_effect_set_ability_animation(0, ability_id, &effect_data);
}
