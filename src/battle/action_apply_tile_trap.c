#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/battle_map.h"
#include "fft/battle_move.h"
#include "psx/types.h"

/* Set up the action result for the trap on a unit's tile.
 *
 * A mount passes the trap to the rider in its linked partner slot. Trap 0
 * levels the target down, 1 and 2 inflict Death Sentence and Sleep (status
 * byte 4 bits 0x01 and 0x10) when status_modify_inflictions allows it, and 3
 * deals max HP / 5 damage. Returns the trap id, or 0xff for none. */
s32 action_apply_tile_trap(battle_stats_t* unit) {
    u8 trap;
    action_data_t* action;

    trap = map_set_item_trap_data(unit->x, unit->position.y, unit->position.higher_elevation)->trap_id;
    if (unit->mount_info & BATTLE_MOUNT_INFO_FLAG_MOUNT) {
        unit = &g_battle_unit_stats[unit->mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK];
    }
    g_action_current_ability.target_id = unit->battle_id;
    g_action_target = unit;
    g_action_target_data = &unit->action;
    action_clear_current_data(&unit->action);
    switch (trap) {
    case BATTLE_TRAP_ID_DEGENERATOR:
        g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
        g_action_target_data->special_effect = ACTION_SPECIAL_EFFECT_LEVEL_DOWN;
        break;
    case BATTLE_TRAP_ID_DEATHTRAP:
        g_action_target_data->status_changes.infliction[STATUS_BYTE_INDEX(STATUS_ID_DEATH_SENTENCE)]
            = STATUS_BYTE_MASK(STATUS_ID_DEATH_SENTENCE);
        if (status_modify_inflictions(0) != 0) {
            g_action_target_data->attack_type = ACTION_TYPE_STATUS_CHANGE;
        } else {
            formula_force_attack_miss();
        }
        break;
    case BATTLE_TRAP_ID_SLEEPING_GAS:
        g_action_target_data->status_changes.infliction[STATUS_BYTE_INDEX(STATUS_ID_SLEEP)]
            = STATUS_BYTE_MASK(STATUS_ID_SLEEP);
        if (status_modify_inflictions(0) != 0) {
            g_action_target_data->attack_type = ACTION_TYPE_STATUS_CHANGE;
        } else {
            formula_force_attack_miss();
        }
        break;
    case BATTLE_TRAP_ID_STEEL_NEEDLE:
        action = g_action_target_data;
        action->hp_damage = unit->max_hp / 5;
        action->attack_type = ACTION_TYPE_HP_DAMAGE;
        break;
    default:
        trap = BATTLE_TRAP_ID_NONE;
        break;
    }
    return trap;
}
