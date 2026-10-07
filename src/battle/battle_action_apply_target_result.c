#include "fft/battle.h"
#include "psx/types.h"

#define STATUS_MASK(id) BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_##id)

/* Apply the target's pending result (its result record) to the unit, and give it its reactions.
 *
 * An absent unit returns -1, and a dead target that the action crystallizes or turns to treasure only
 * has its statuses resolved. In a primary action ability_outcome becomes 1; the target's reaction is
 * dispatched (battle_reaction_check_when_targeted) and a Catch returns the caught item to
 * the party. A miss stops there, and a Golem guard takes the damage from the team's Golem HP instead.
 * Otherwise, in order: a Golem is set to the target's max HP; MP Switch, Distribute and Damage Split
 * get their chance; HP and MP change (clamped to 0..max); Speed (1-50), CT (0-255), PA and MA (1-99),
 * Brave (0-100, at least 10 for a rider, so a rider never turns Chicken) and Faith (0-100) change;
 * broken or stolen equipment leaves; an unbroken Draw Out katana returns; gil, EXP and JP change;
 * Level Up/Down applies; a Poach adds the monster's item to the Fur Shop; and the MORBOL effect, while
 * executing, reapplies the target's status flags (battle_status_clear_all).
 *
 * A target brought to 0 HP gets Dead (its KO count grows while executing) and returns -1 when it is a
 * rider, else 0. Otherwise Critical follows HP <= max HP / 5, Chicken follows Brave < 10, HP damage
 * removes Charm, Sleep, Transparent and Confusion, a knockback can cancel the target's charge
 * (battle_status_check_charging_charge), and the status changes resolve. The outcome (2 newly
 * dead, 1 anything changed, 0 nothing) goes to the attacker's EXP award
 * (battle_unit_update_attacker_earned_experience), ability_outcome becomes 2 when something changed,
 * and the stat-save and restore reactions get their chance. Returns -1 when a rider ends dead or
 * crystallized, else 0.
 *
 * The KO test's three status-bit checks fold into the target's single halfword load of
 * current[0..1] & 0x160. */
s32 battle_action_apply_target_result(s32 unit_id) {
    s32 outcome;
    s32 hp;
    s32 mp;
    s32 i;
    u16 old_hp;
    u16 old_mp;
    u8 old_status[5];
    u8 old_status_ct[16];

    outcome = 0;
    if (unit_id >= BATTLE_UNIT_SLOT_COUNT) {
        return -1;
    }
    g_current_ability.target_id = unit_id;
    g_battle_action_target = &g_battle_unit_stats[unit_id];
    g_battle_action_target_data = &g_battle_unit_stats[unit_id].action;
    if (g_battle_action_target->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return -1;
    }
    if ((g_battle_action_target->status_sets.current[0] & STATUS_MASK(DEAD))
        && ((g_battle_action_target_data->status_infliction[0] & STATUS_MASK(CRYSTAL))
            || (g_battle_action_target_data->status_infliction[1] & STATUS_MASK(TREASURE)))) {
        battle_status_resolve_unit_changes(unit_id, 0);
        return 0;
    }
    if (g_battle_action_context == BATTLE_ACTION_CONTEXT_PRIMARY) {
        g_battle_action_target->ability_outcome = 1;
    }
    battle_reaction_check_when_targeted();
    if (g_battle_action_target_data->reaction_id == ABILITY_ID_REACTION_CATCH) {
        battle_action_add_party_item(g_battle_action_target, (u8)g_battle_action_target_data->last_received_attack);
    }
    if (g_battle_action_target_data->hit == 0) {
        return 0;
    }
    if (g_battle_action_target_data->special_effect & BATTLE_ACTION_SPECIAL_EFFECT_GOLEM_GUARD) {
        u16* golem;

        golem = &g_battle_team_golem[(g_battle_action_target->initial_team_flags & BATTLE_TEAM_MASK) >> 4];
        hp = *golem - g_battle_action_target_data->hp_damage;
        if (hp < 0) {
            hp = 0;
        }
        *golem = hp;
        return 0;
    }
    if (g_battle_action_target_data->special_effect & BATTLE_ACTION_SPECIAL_EFFECT_SET_GOLEM) {
        outcome = 1;
        g_battle_team_golem[(g_battle_action_target->team_flags & BATTLE_TEAM_MASK) >> 4]
            = g_battle_action_target->max_hp;
    }
    battle_reaction_check_before_hp_change();
    old_hp = g_battle_action_target->hp;
    hp = old_hp - g_battle_action_target_data->hp_damage + g_battle_action_target_data->hp_healing;
    if (hp < 0) {
        hp = 0;
    }
    if (g_battle_action_target->max_hp < hp) {
        hp = g_battle_action_target->max_hp;
    }
    old_mp = g_battle_action_target->mp;
    mp = old_mp - g_battle_action_target_data->mp_damage + g_battle_action_target_data->mp_healing;
    if (mp < 0) {
        mp = 0;
    }
    if (g_battle_action_target->max_mp < mp) {
        mp = g_battle_action_target->max_mp;
    }
    if (old_hp != hp || old_mp != mp) {
        outcome = 1;
    }
    g_battle_action_target->hp = hp;
    g_battle_action_target->mp = mp;
    outcome |= battle_unit_apply_stat_increment_decrement(
        g_battle_action_target_data->sp_change, &g_battle_action_target->base_attributes[UNIT_ATTRIBUTE_SPEED], 50, 1);
    outcome |= battle_unit_apply_stat_increment_decrement(
        g_battle_action_target_data->ct_change, &g_battle_action_target->ct, 255, 0);
    outcome |= battle_unit_apply_stat_increment_decrement(g_battle_action_target_data->pa_change,
        &g_battle_action_target->base_attributes[UNIT_ATTRIBUTE_PHYSICAL_ATTACK], 99, 1);
    outcome |= battle_unit_apply_stat_increment_decrement(g_battle_action_target_data->ma_change,
        &g_battle_action_target->base_attributes[UNIT_ATTRIBUTE_MAGIC_ATTACK], 99, 1);
    outcome |= battle_unit_apply_stat_increment_decrement(g_battle_action_target_data->brave_change,
        &g_battle_action_target->brave, 100,
        (g_battle_action_target->mount_info & BATTLE_MOUNT_INFO_FLAG_RIDER) ? 10 : 0);
    outcome |= battle_unit_apply_stat_increment_decrement(
        g_battle_action_target_data->faith_change, &g_battle_action_target->faith, 100, 0);
    outcome |= battle_action_remove_broken_or_stolen_equipment();
    if (g_battle_action_target_data->special_effect & BATTLE_ACTION_SPECIAL_EFFECT_DRAW_OUT_KATANA_NOT_BROKEN) {
        battle_action_add_party_item(g_battle_action_target, (u8)g_battle_action_target->used_item_or_equipment);
    }
    if (g_battle_action_target_data->gil_change != 0 || g_battle_action_target_data->exp_change != 0) {
        outcome |= 1;
    }
    battle_action_add_war_funds(g_battle_action_target, g_battle_action_target_data->gil_change, 0);
    battle_action_apply_exp_change(g_battle_action_target, g_battle_action_target_data->exp_change);
    if (g_battle_action_target_data->jp_change != 0) {
        battle_action_apply_jp_change(g_battle_action_target);
    }
    outcome |= battle_unit_apply_level_up_down_ability();
    outcome |= battle_action_add_poached_item_to_fur_shop_inventory();
    if (g_battle_action_target_data->special_effect & BATTLE_ACTION_SPECIAL_EFFECT_MORBOL) {
        outcome |= 1;
        if (g_battle_action_state == BATTLE_ACTION_STATE_EXECUTE) {
            battle_status_clear_all(g_battle_action_target);
        }
    }
    if (hp == 0 && !(g_battle_action_target->status_sets.current[0] & STATUS_MASK(CRYSTAL))
        && !(g_battle_action_target->status_sets.current[0] & STATUS_MASK(DEAD))
        && !(g_battle_action_target->status_sets.current[1] & STATUS_MASK(TREASURE))) {
        battle_action_clear_status_changes(g_battle_action_target_data);
        g_battle_action_target_data->status_infliction[0] = STATUS_MASK(DEAD);
        battle_status_resolve_unit_changes(unit_id, 1);
        if (g_battle_action_state == BATTLE_ACTION_STATE_EXECUTE) {
            g_battle_action_target->ko_count++;
        }
        battle_unit_update_attacker_earned_experience(2);
        return -(g_battle_action_target->mount_info >> 7);
    }
    if (hp <= (u16)(g_battle_action_target->max_hp / 5)) {
        g_battle_action_target_data->status_infliction[2] |= STATUS_MASK(CRITICAL);
    } else {
        g_battle_action_target_data->status_removal[2] |= STATUS_MASK(CRITICAL);
    }
    if (g_battle_action_target->brave < 10) {
        g_battle_action_target_data->status_infliction[2] |= STATUS_MASK(CHICKEN);
    } else {
        g_battle_action_target_data->status_removal[2] |= STATUS_MASK(CHICKEN);
    }
    if (g_battle_action_target_data->attack_type & BATTLE_ACTION_TYPE_HP_DAMAGE) {
        g_battle_action_target_data->status_removal[4] |= STATUS_MASK(CHARM) | STATUS_MASK(SLEEP);
        g_battle_action_target_data->status_removal[2] |= STATUS_MASK(TRANSPARENT);
        g_battle_action_target_data->status_removal[1] |= STATUS_MASK(CONFUSION);
    }
    if ((g_battle_action_target_data->special_effect & BATTLE_ACTION_SPECIAL_EFFECT_KNOCKBACK)
        && battle_status_check_charging_charge(g_battle_action_target, 0) != 0) {
        g_battle_action_target_data->status_removal[0] |= STATUS_MASK(CHARGING);
        g_battle_action_target->charged_ability_ct = 0xff;
    }
    battle_status_modify_inflictions(0);
    for (i = 0; i < 5; i++) {
        old_status[i] = g_battle_action_target->status_sets.current[i];
    }
    for (i = 0; i < 16; i++) {
        old_status_ct[i] = g_battle_action_target->status_ct[i];
    }
    battle_status_resolve_unit_changes(unit_id, 0);
    if ((g_battle_action_target->status_sets.current[0] & STATUS_MASK(DEAD)) && !(old_status[0] & STATUS_MASK(DEAD))) {
        outcome = 2;
        if (g_battle_action_state == BATTLE_ACTION_STATE_EXECUTE) {
            g_battle_action_target->ko_count++;
        }
    } else {
        for (i = 0; i < 5; i++) {
            if (g_battle_action_target->status_sets.current[i] != old_status[i]) {
                outcome = 1;
                break;
            }
        }
        for (i = 0; i < 16; i++) {
            if (g_battle_action_target->status_ct[i] != old_status_ct[i]) {
                outcome = 1;
                break;
            }
        }
    }
    battle_unit_update_attacker_earned_experience(outcome);
    if (outcome != 0) {
        g_battle_action_target->ability_outcome = 2;
    }
    battle_reaction_check_after_result();
    if (g_battle_action_target->status_sets.current[0] & (STATUS_MASK(CRYSTAL) | STATUS_MASK(DEAD))) {
        if (g_battle_action_target->mount_info & BATTLE_MOUNT_INFO_FLAG_RIDER) {
            return -1;
        }
    }
    return 0;
}
