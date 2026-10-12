#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_effect.h"
#include "fft/battle_unit.h"
#include "psx/types.h"

/* Fill a secondary-effect init record with the unit's current action targets.
 *
 * Each target block records the target's unit id and a result animation
 * derived from its action data. With no targets the first block instead holds
 * the cursor tile position. The record's target blocks form an array of up to
 * 16 entries starting at the header's first `target` block. */
void battle_effect_build_secondary_init_from_action(
    battle_unit_misc_data_t* unit, battle_effect_secondary_init_t* init) {
    battle_unit_misc_data_t* target;
    battle_action_data_t* action;
    s32 i;

    init->target_count = unit->strike.target_count;
    init->targets[BATTLE_EFFECT_CASTER_ENTRY].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    init->targets[BATTLE_EFFECT_CASTER_ENTRY].data.unit.misc_id = unit->unit_id;
    init->used_weapon_id = unit->strike.used_weapon_id;
    if (init->target_count != 0) {
        for (i = 0; i < init->target_count; i++) {
            target = battle_unit_get_misc_data_by_battle_id(unit->strike.target_list[i]);
            init->targets[i].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
            init->targets[i].data.unit.misc_id = target->unit_id;
            action = &target->battle_data->action;
            switch (action->miss_type) {
            case BATTLE_ACTION_MISS_TYPE_ACCESSORY_EVADE:
            case BATTLE_ACTION_MISS_TYPE_RIGHT_HAND_EVADE:
            case BATTLE_ACTION_MISS_TYPE_LEFT_HAND_EVADE:
            case BATTLE_ACTION_MISS_TYPE_CLASS_EVADE_OR_ARROW_GUARD:
                init->targets[i].data.unit.result_animation = BATTLE_EFFECT_HIT_MISSED;
                break;
            case BATTLE_ACTION_MISS_TYPE_NULLIFIED:
            case BATTLE_ACTION_MISS_TYPE_ACCURACY_MISS:
            case BATTLE_ACTION_MISS_TYPE_FORCED_FAILURE:
            case BATTLE_ACTION_MISS_TYPE_CANCELLED:
            case BATTLE_ACTION_MISS_TYPE_BLADE_GRASP_OR_FINGER_GUARD:
            case BATTLE_ACTION_MISS_TYPE_CATCH:
                init->targets[i].data.unit.result_animation = BATTLE_EFFECT_HIT_NO_EFFECT;
                break;
            case BATTLE_ACTION_MISS_TYPE_GOLEM_GUARD:
                init->targets[i].data.unit.result_animation = BATTLE_EFFECT_HIT_GOLEM;
                break;
            default:
                if (action->special_effect & BATTLE_ACTION_SPECIAL_EFFECT_BREAK_EQUIPMENT) {
                    init->targets[i].data.unit.result_animation = BATTLE_EFFECT_HIT_BREAK;
                } else if (action->critical != 0) {
                    init->targets[i].data.unit.result_animation = BATTLE_EFFECT_HIT_CRITICAL;
                } else {
                    init->targets[i].data.unit.result_animation = BATTLE_EFFECT_HIT_NORMAL;
                }
                break;
            }
        }
    } else {
        init->targets[0].target_type = BATTLE_EFFECT_TARGET_TYPE_TILE;
        init->targets[0].data.tile.vx = g_battle_cursor_x;
        init->targets[0].data.tile.vy = g_battle_cursor_z;
        init->targets[0].data.tile.vz = g_battle_cursor_y;
    }
}
