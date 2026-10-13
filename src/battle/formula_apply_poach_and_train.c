#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Resolve Secret Hunt and Train after an HP-damaging formula 01-06 hit.
 *
 * Only monsters (neither sex bit set) qualify, and not through a team Golem
 * while the ability can be blocked by it, nor when dead, petrified or
 * walled. A killing blow from a Secret Hunt attacker poaches a monster job
 * target; a hit leaving the target at or below 1/5 max HP lets a Train
 * attacker inflict Invite. */
void formula_apply_poach_and_train(void) {
    action_data_t* action;
    unit_stats_t* target;
    s32 remaining;

    action = g_action_target_data;
    if (!(action->attack_type & ACTION_TYPE_HP_DAMAGE)) {
        return;
    }
    target = g_action_target;
    remaining = target->hp - action->hp_damage + action->hp_healing;
    if (target->unit_flags & (UNIT_FLAG_FEMALE | UNIT_FLAG_MALE)) {
        return;
    }
    if ((g_battle_team_golem[(target->initial_team_flags & BATTLE_TEAM_MASK) >> BATTLE_TEAM_SHIFT] != 0)
        && (g_action_current_ability.range_data.flags_3 & ABILITY_SECONDARY_FLAG_3_BLOCKED_BY_GOLEM)) {
        return;
    }
    if ((target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_DEAD)] & STATUS_BYTE_MASK(STATUS_ID_DEAD))
        || (target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_PETRIFY)] & STATUS_BYTE_MASK(STATUS_ID_PETRIFY))) {
        return;
    }
    if (target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_WALL)] & STATUS_BYTE_MASK(STATUS_ID_WALL)) {
        return;
    }
    if (((remaining <= 0)
            || (action->status_changes.infliction[STATUS_BYTE_INDEX(STATUS_ID_DEAD)]
                & STATUS_BYTE_MASK(STATUS_ID_DEAD)))
        && (g_action_attacker->ability_flags.support[2] & BATTLE_SUPPORT_SET_3_SECRET_HUNT)
        && !(target->team_flags & BATTLE_TEAM_FLAG_IMMORTAL)) {
        if (target->job_id < JOB_ID_MONSTER_FIRST || target->job_id > JOB_ID_MONSTER_LAST) {
            return;
        }
        action_clear_knockback_flag();
        formula_clear_nullify_flags();
        g_action_target_data->special_effect |= ACTION_SPECIAL_EFFECT_POACH;
        g_action_current_ability.post_action_message = BATTLE_POST_ACTION_MESSAGE_POACHED;
        return;
    }
    if ((remaining > 0) && ((g_action_target->max_hp / 5) >= remaining)
        && (g_action_attacker->ability_flags.support[2] & BATTLE_SUPPORT_SET_3_TRAIN)) {
        g_action_target_data->status_changes.infliction[STATUS_BYTE_INDEX(STATUS_ID_INVITE)]
            |= STATUS_BYTE_MASK(STATUS_ID_INVITE);
        if (status_modify_inflictions(0) != 0) {
            g_action_target_data->special_effect |= ACTION_SPECIAL_EFFECT_TEAM_CHANGE;
            g_action_target_data->attack_type |= ACTION_TYPE_STATUS_CHANGE;
            g_action_current_ability.post_action_message = BATTLE_POST_ACTION_MESSAGE_TRAINED;
        }
    }
}
