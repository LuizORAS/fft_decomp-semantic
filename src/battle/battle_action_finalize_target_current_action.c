#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Validate and clamp the target's result before it applies.
 *
 * A target that is dead (unless the ability cancels Dead and the target is not undead), petrified or
 * under Wall, without the ability cancelling that status, turns the action into a cancelled miss and
 * leaves the target list. Otherwise zero accuracy clears the hit, HP/MP changes are capped at 999, a
 * lethal HP hit cancels the knockback and the weapon spell, and in a primary action a hit that the
 * target team's Golem absorbs (an ability blocked by Golem) becomes a Golem guard miss. */
void battle_action_finalize_target_current_action(void) {
    battle_action_data_t* action;
    u8 status;
    s32 invalid;
    s32 team;

    status = g_battle_action_target->status_sets.current[0];
    invalid = 0;
    if ((status & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD))
        && (!(g_battle_action_canceled_statuses[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD))
            || (status & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_UNDEAD)))) {
        invalid = 1;
    }
    if ((g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PETRIFY)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PETRIFY))
        && !(g_battle_action_canceled_statuses[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PETRIFY)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PETRIFY))) {
        invalid = 1;
    }
    if ((g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_WALL)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_WALL))
        && !(g_battle_action_canceled_statuses[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_WALL)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_WALL))) {
        invalid = 1;
    }
    if (invalid != 0) {
        battle_target_remove_ability_target(g_battle_action_target->battle_id);
        battle_action_clear_target_and_actor_data();
        battle_formula_cancel_weapon_spell();
        battle_action_cancel_knockback();
        g_battle_action_current_ability.post_action_message = BATTLE_POST_ACTION_MESSAGE_NONE;
        battle_formula_force_attack_miss();
        g_battle_action_target_data->miss_type = BATTLE_ACTION_MISS_TYPE_CANCELLED;
        return;
    }
    if (g_battle_action_target_data->attack_accuracy == 0) {
        g_battle_action_target_data->hit = 0;
    }
    if (g_battle_action_target_data->hp_damage >= 1000) {
        g_battle_action_target_data->hp_damage = 999;
    }
    if (g_battle_action_target_data->hp_healing >= 1000) {
        g_battle_action_target_data->hp_healing = 999;
    }
    if (g_battle_action_target_data->mp_damage >= 1000) {
        g_battle_action_target_data->mp_damage = 999;
    }
    if (g_battle_action_target_data->mp_healing >= 1000) {
        g_battle_action_target_data->mp_healing = 999;
    }
    if (g_battle_action_target_data->hp_damage >= g_battle_action_target->hp) {
        battle_action_cancel_knockback();
        battle_formula_cancel_weapon_spell();
    }
    team = (g_battle_action_target->initial_team_flags & BATTLE_TEAM_MASK) >> BATTLE_TEAM_SHIFT;
    action = g_battle_action_target_data;
    if ((action->attack_type != 0) && (action->hit != 0) && (g_battle_action_context == BATTLE_ACTION_CONTEXT_PRIMARY)
        && (g_battle_team_golem[team] != 0)
        && (g_battle_action_current_ability.range_data.flags_3 & ABILITY_SECONDARY_FLAG_3_BLOCKED_BY_GOLEM)) {
        action->miss_type = BATTLE_ACTION_MISS_TYPE_GOLEM_GUARD;
        action->special_effect = BATTLE_ACTION_SPECIAL_EFFECT_GOLEM_GUARD;
        battle_action_clear_status_changes(g_battle_action_target_data);
        battle_action_cancel_knockback();
    }
}
