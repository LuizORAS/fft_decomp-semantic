#include "fft/battle.h"
#include "psx/types.h"

/* Give the single unit targeted by id's pending action the chance to strike first with Hamedo
 * (First Strike), reporting the reaction through out_ability. The actor must be able to act (its MP
 * is only tested) with a weapon strike (Attack, Charge, Throw or a weapon-range ability) on a valid
 * target, and exactly one other unit must stand on the hit tiles; that unit needs Hamedo, a successful
 * Brave roll and the freedom to react. It then counters with Attack at the actor and pays its MP;
 * g_current_ability_hamedo_flag cancels the actor's action, and a Mime actor gives back its lent
 * weapons. Returns the reacting unit's slot, or -1 when nothing reacts.
 *
 * battle_target_validate_weapon_target takes only the action record (it copies the record and
 * rederives the menu type itself); menu_type and flags_1 merely happen to sit
 * in $a1/$a2 at that call. Passing them as arguments would set $a2 twice and
 * change the scheduling of the counter-ability call's argument setup. */
s32 battle_reaction_prepare_hamedo_for_pending_action(s32 id, u16* out_ability) {
    u8 targets[BATTLE_UNIT_SLOT_COUNT];
    battle_stats_t* unit;
    battle_stats_t* base;
    battle_stats_t* target;
    u8* action;
    s32 i;
    s32 count;
    s32 result;
    s32 menu_type;
    u8 flags_1;
    u16 mp;
    s32 tid;
    u16 ability_id;

    unit = &g_battle_unit_stats[id];
    base = g_battle_unit_stats;
    g_current_ability_hamedo_flag = 0;
    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return -1;
    }
    mp = unit->mp;
    result = battle_action_check_and_consume_mp(unit);
    unit->mp = mp;
    if (result != 0) {
        return -1;
    }

    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        base[i].action.reaction_id = 0;
        base[i].ability_outcome = 0;
    }

    menu_type = g_main_action_menu_types_by_skillset[unit->last_skillset_id];
    flags_1 = 0;
    if ((menu_type & 0xff) == ACTION_MENU_TYPE_DEFAULT || (menu_type & 0xff) == ACTION_MENU_TYPE_MONSTER) {
        ability_id = *(u16*)&unit->last_ability_id;
        if (ability_id < ABILITY_ID_ITEM_FIRST) {
            flags_1 = g_main_ability_range_data[ability_id].flags_1;
        }
    }

    result = -1;
    if ((menu_type & 0xff) == ACTION_MENU_TYPE_ATTACK || (menu_type & 0xff) == ACTION_MENU_TYPE_CHARGE
        || (menu_type & 0xff) == ACTION_MENU_TYPE_WEAPON_INVENTORY
        || (flags_1 & ABILITY_SECONDARY_FLAG_1_WEAPON_RANGE)) {
        result = battle_target_validate_weapon_target((battle_ai_command_action_t*)&unit->action_actor_id);
    }
    if (result == -1) {
        return -1;
    }

    g_battle_sort_targets_nearest_first = 0;
    count = battle_target_list_units_on_panels(targets, unit);
    if (count != 1) {
        return -1;
    }
    tid = targets[0];
    if (tid == id) {
        return -1;
    }
    target = &g_battle_unit_stats[tid];
    if (!(target->reaction_abilities[3] & BATTLE_REACTION_SET_4_HAMEDO)) {
        return -1;
    }
    if (battle_reaction_fails_brave_roll(target) != 0) {
        return -1;
    }
    action = &target->action_actor_id;
    if (battle_reaction_is_prevented(target) != 0) {
        return -1;
    }

    g_battle_action_context = count;
    main_util_copy_action_data(action, g_reaction_unit_action_data_16e);
    g_battle_current_reaction_ability_id = ABILITY_ID_REACTION_HAMEDO;
    g_battle_acting_unit_id = id;
    target->action_actor_id = tid;
    if (battle_reaction_set_counter_command(target, SKILLSET_ID_ATTACK, 0, 1) == 0
        && battle_action_check_and_consume_mp(target) == 0) {
        battle_action_init_current_ability_strike_data(target);
        *out_ability = *(u16*)&g_battle_current_reaction_ability_id;
        g_current_ability_hamedo_flag = count;
        if (unit->job_id == JOB_ID_MIME) {
            unit->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON] = ITEM_ID_NONE;
            unit->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON] = ITEM_ID_NONE;
        }
        return tid;
    }
    main_util_copy_action_data(g_reaction_unit_action_data_16e, action);
    return -1;
}
