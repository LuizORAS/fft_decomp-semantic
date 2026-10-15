#include "fft/battle.h"
#include "psx/types.h"

/* Starts an ability's effect outside an action, on a unit (target_data is its
 * misc id), on a tile (target_data points to its x, level and y halfwords) or
 * on a unit that is also its caster. */
void battle_effect_start_new(s32 ability_id, s32 target_mode, u8* target_data) {
    battle_effect_secondary_init_t effect_data;

    effect_data.target_count = 1;
    effect_data.caster.fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    effect_data.caster.fields.caster_id = 0;

    if (target_mode == BATTLE_EFFECT_START_ON_UNIT) {
        effect_data.target[0].fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
        effect_data.target[0].fields.target_id = (u32)target_data;
        effect_data.target[0].fields.result_animation = BATTLE_EFFECT_HIT_NORMAL;
    } else if (target_mode == BATTLE_EFFECT_START_ON_TILE) {
        effect_data.target[0].fields.target_type = BATTLE_EFFECT_TARGET_TYPE_TILE;
        effect_data.target[0].block.values[1] = *(u16*)&target_data[0];
        effect_data.target[0].block.values[3] = *(u16*)&target_data[4];
        effect_data.target[0].block.values[2] = *(u16*)&target_data[2];
    } else {
        effect_data.target[0].fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
        effect_data.target[0].fields.target_id = (u32)target_data;
        effect_data.target[0].fields.result_animation = BATTLE_EFFECT_HIT_NORMAL;
        effect_data.caster.fields.caster_id = (u32)target_data;
    }

    effect_data.spectator_count = 0;
    battle_effect_request_ability_effect(0, ability_id, &effect_data);
}
