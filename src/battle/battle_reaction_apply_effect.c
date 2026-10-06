#include "fft/battle.h"

/* The switch runs on the offset from the first reaction ability. */
#define REACTION_CASE(name) (ABILITY_ID_REACTION_##name - ABILITY_ID_REACTION_FIRST)

/* Fill the reacting unit's result with its reaction's own effect, and finish it: PA, MA or Speed
 * Save +1; Sunken State Transparent, Caution Defending, Dragon Spirit Reraise and Regenerator Regen
 * (when they can be inflicted); Brave Up and Faith Up +3; HP Restore and MP Restore to full; Critical
 * Quick a full CT; Meatbone Slash damage equal to the reacting unit's max HP; Absorb Used MP the MP
 * cost back; Gilgame Heart the damage as gil; Distribute an equal share of the excess healing
 * (rounding up) to each ally; Damage Split half the damage to the actor, healing the reacting unit by
 * as much. For Reflect the acting unit is the attacker again. Returns 0 when the reaction's behaviour
 * flags send it through an ability instead, -1 for a reaction without an effect here, else 1.
 *
 * Case 0x18 chains the damage store so the value reaches `healing` through
 * the target record; storing it there first is what orders the three global
 * pointer loads the way the target emits them. */
s32 battle_reaction_apply_effect(void) {
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
    if ((g_main_reaction_behavior_flags_by_ability_id[ability_id]
            & (BATTLE_REACTION_BEHAVIOR_FLAG_TRIGGER_ABILITY | BATTLE_REACTION_BEHAVIOR_FLAG_ABILITY))
        == BATTLE_REACTION_BEHAVIOR_FLAG_TRIGGER_ABILITY) {
        return 0;
    }
    battle_action_clear_target_and_actor_data();
    selector = (u16)g_battle_current_reaction_ability_id;
    switch ((s16)(selector - ABILITY_ID_REACTION_FIRST)) {
    case REACTION_CASE(PA_SAVE):
        g_battle_action_target_data->pa_change = BATTLE_ACTION_STAT_CHANGE_INCREASE | 1;
        break;
    case REACTION_CASE(MA_SAVE):
        g_battle_action_target_data->ma_change = BATTLE_ACTION_STAT_CHANGE_INCREASE | 1;
        break;
    case REACTION_CASE(SPEED_SAVE):
        g_battle_action_target_data->sp_change = BATTLE_ACTION_STAT_CHANGE_INCREASE | 1;
        break;
    case REACTION_CASE(SUNKEN_STATE):
        g_battle_action_target_data->status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TRANSPARENT)]
            = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT);
        if (battle_status_modify_inflictions(0) != 0) {
            attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
        }
        break;
    case REACTION_CASE(CAUTION):
        g_battle_action_target_data->status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEFENDING)]
            = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEFENDING);
        if (battle_status_modify_inflictions(0) != 0) {
            attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
        }
        break;
    case REACTION_CASE(DRAGON_SPIRIT):
        g_battle_action_target_data->status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_RERAISE)]
            = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_RERAISE);
        if (battle_status_modify_inflictions(0) != 0) {
            attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
        }
        break;
    case REACTION_CASE(REGENERATOR):
        g_battle_action_target_data->status_infliction[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_REGEN)]
            = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_REGEN);
        if (battle_status_modify_inflictions(0) != 0) {
            attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
        }
        break;
    case REACTION_CASE(BRAVE_UP):
        g_battle_action_target_data->brave_change = BATTLE_ACTION_STAT_CHANGE_INCREASE | 3;
        break;
    case REACTION_CASE(FAITH_UP):
        g_battle_action_target_data->faith_change = BATTLE_ACTION_STAT_CHANGE_INCREASE | 3;
        break;
    case REACTION_CASE(HP_RESTORE):
        attack_type = BATTLE_ACTION_TYPE_HP_HEALING;
        g_battle_action_target_data->hp_healing = g_battle_action_target->max_hp - g_battle_action_target->hp;
        break;
    case REACTION_CASE(MP_RESTORE):
        attack_type = BATTLE_ACTION_TYPE_MP_HEALING;
        g_battle_action_target_data->mp_healing = g_battle_action_target->max_mp - g_battle_action_target->mp;
        break;
    case REACTION_CASE(CRITICAL_QUICK):
        g_battle_action_target_data->ct_change
            = BATTLE_ACTION_STAT_CHANGE_INCREASE | BATTLE_ACTION_STAT_CHANGE_VALUE_MASK;
        break;
    case REACTION_CASE(MEATBONE_SLASH):
        attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
        g_battle_action_target_data->hp_damage = g_battle_action_attacker->max_hp;
        break;
    case REACTION_CASE(ABSORB_USED_MP):
        attack_type = BATTLE_ACTION_TYPE_MP_HEALING;
        g_battle_action_target_data->mp_healing = g_battle_action_target_data->last_received_attack;
        break;
    case REACTION_CASE(GILGAME_HEART):
        g_battle_action_target_data->gil_change = g_battle_action_target_data->last_received_attack;
        break;
    case REACTION_CASE(DISTRIBUTE):
        count = g_battle_distribute_target_count;
        if (count != 0) {
            healing = (g_battle_action_attacker->action.last_received_attack + count - 1) / count;
        } else {
            healing = 0;
        }
        attack_type = BATTLE_ACTION_TYPE_HP_HEALING;
        g_battle_action_target_data->hp_healing = healing;
        break;
    case REACTION_CASE(DAMAGE_SPLIT):
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
    g_current_ability.reaction_id = 0;
    return 1;
}
