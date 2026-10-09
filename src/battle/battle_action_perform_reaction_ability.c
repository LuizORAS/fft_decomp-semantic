#include "fft/battle.h"

/* Applies the pending reaction ability's effect to the current target action
 * record and finishes the strike. Returns 0 when the ability's behaviour
 * flags select the alternate handler, -1 for an ability with no effect entry,
 * and 1 once the action has been finalised.
 *
 * Case 0x18 chains the damage store so the value reaches `healing` through
 * the target record; storing it there first is what orders the three global
 * pointer loads the way the target emits them. */
s32 battle_action_perform_reaction_ability(void) {
    s16 ability_id;
    u8 attack_type;
    s32 healing;
    s32 selector;
    s32 count;

    attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
    ability_id = g_battle_current_reaction_ability_id;
    if (ability_id == ABILITY_ID_REACTION_REFLECT) {
        g_battle_action_attacker = &g_battle_unit_stats[g_battle_acting_unit_id];
    }
    if ((g_main_reaction_behavior_flags[ability_id - ABILITY_ID_REACTION_FIRST]
            & (BATTLE_REACTION_BEHAVIOR_FLAG_TRIGGER_ABILITY | BATTLE_REACTION_BEHAVIOR_FLAG_ABILITY))
        == 1) {
        return 0;
    }
    battle_action_clear_data();
    selector = (u16)g_battle_current_reaction_ability_id;
    switch ((s16)(selector - ABILITY_ID_REACTION_FIRST)) {
    case ABILITY_ID_REACTION_PA_SAVE - ABILITY_ID_REACTION_FIRST:
        g_battle_action_target_data->pa_change = 0x81;
        break;
    case ABILITY_ID_REACTION_MA_SAVE - ABILITY_ID_REACTION_FIRST:
        g_battle_action_target_data->ma_change = 0x81;
        break;
    case ABILITY_ID_REACTION_SPEED_SAVE - ABILITY_ID_REACTION_FIRST:
        g_battle_action_target_data->sp_change = 0x81;
        break;
    case ABILITY_ID_REACTION_SUNKEN_STATE - ABILITY_ID_REACTION_FIRST:
        g_battle_action_target_data->status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TRANSPARENT)]
            = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT);
        if (battle_status_modify_inflictions(0) != 0) {
            attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
        }
        break;
    case ABILITY_ID_REACTION_CAUTION - ABILITY_ID_REACTION_FIRST:
        g_battle_action_target_data->status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEFENDING)]
            = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEFENDING);
        if (battle_status_modify_inflictions(0) != 0) {
            attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
        }
        break;
    case ABILITY_ID_REACTION_DRAGON_SPIRIT - ABILITY_ID_REACTION_FIRST:
        g_battle_action_target_data->status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_RERAISE)]
            = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_RERAISE);
        if (battle_status_modify_inflictions(0) != 0) {
            attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
        }
        break;
    case ABILITY_ID_REACTION_REGENERATOR - ABILITY_ID_REACTION_FIRST:
        g_battle_action_target_data->status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_REGEN)]
            = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_REGEN);
        if (battle_status_modify_inflictions(0) != 0) {
            attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
        }
        break;
    case ABILITY_ID_REACTION_BRAVE_UP - ABILITY_ID_REACTION_FIRST:
        g_battle_action_target_data->brave_change = 0x83;
        break;
    case ABILITY_ID_REACTION_FAITH_UP - ABILITY_ID_REACTION_FIRST:
        g_battle_action_target_data->faith_change = 0x83;
        break;
    case ABILITY_ID_REACTION_HP_RESTORE - ABILITY_ID_REACTION_FIRST:
        attack_type = BATTLE_ACTION_TYPE_HP_HEALING;
        g_battle_action_target_data->hp_healing = g_battle_action_target->max_hp - g_battle_action_target->hp;
        break;
    case ABILITY_ID_REACTION_MP_RESTORE - ABILITY_ID_REACTION_FIRST:
        attack_type = BATTLE_ACTION_TYPE_MP_HEALING;
        g_battle_action_target_data->mp_healing = g_battle_action_target->max_mp - g_battle_action_target->mp;
        break;
    case ABILITY_ID_REACTION_CRITICAL_QUICK - ABILITY_ID_REACTION_FIRST:
        g_battle_action_target_data->ct_change = BATTLE_ACTION_CT_CHANGE_QUICK;
        break;
    case ABILITY_ID_REACTION_MEATBONE_SLASH - ABILITY_ID_REACTION_FIRST:
        attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
        g_battle_action_target_data->hp_damage = g_battle_action_attacker->max_hp;
        break;
    case ABILITY_ID_REACTION_ABSORB_USED_MP - ABILITY_ID_REACTION_FIRST:
        attack_type = BATTLE_ACTION_TYPE_MP_HEALING;
        g_battle_action_target_data->mp_healing = g_battle_action_target_data->last_received_attack;
        break;
    case ABILITY_ID_REACTION_GILGAME_HEART - ABILITY_ID_REACTION_FIRST:
        g_battle_action_target_data->gil_change = g_battle_action_target_data->last_received_attack;
        break;
    case ABILITY_ID_REACTION_DISTRIBUTE - ABILITY_ID_REACTION_FIRST:
        count = g_battle_distribute_target_count;
        if (count != 0) {
            healing = (g_battle_action_attacker->action.last_received_attack + count - 1) / count;
        } else {
            healing = 0;
        }
        attack_type = BATTLE_ACTION_TYPE_HP_HEALING;
        g_battle_action_target_data->hp_healing = healing;
        break;
    case ABILITY_ID_REACTION_DAMAGE_SPLIT - ABILITY_ID_REACTION_FIRST:
        healing = g_battle_action_target_data->hp_damage = g_battle_action_attacker->action.last_received_attack;
        attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
        g_battle_action_attacker_data->hp_healing = healing;
        g_battle_action_attacker_data->attack_type = BATTLE_ACTION_TYPE_HP_HEALING;
        g_battle_action_attacker_data->hit = 1;
        break;
    default:
        g_battle_action_target_data->attack_type = 0;
        return -1;
    }
    g_battle_action_target_data->attack_type = attack_type;
    battle_action_finalize_target_current_action();
    if (g_battle_action_target_data->special_effect != 0) {
        g_battle_action_target_data->attack_type |= BATTLE_ACTION_TYPE_PSEUDO_STATUS;
    }
    g_battle_action_current_ability.reaction_id = 0;
    return 1;
}
