#include "fft/battle.h"
#include "fft/battle_menu.h"
#include "fft/thread.h"
#include "psx/types.h"

/*
 * Attack-preview caster panel: bind the selected unit's pending action to the
 * previewed target, run the preview calculation on the main stack unless the
 * ability's secondary flag 0x04 is set or the skillset changes equipment, and publish the
 * target's action results through g_battle_menu_preview_target_action (0 when skipped).
 */
void battle_menu_preview_attack_caster_stats(void) {
    battle_stats_t* attacker;
    battle_stats_t* target;
    action_command_t* action;
    ability_data_t* ability_data;
    main_ability_secondary_pointer_t secondary;
    s16* selected;

    attacker = battle_unit_get_attacker_data_pointer();
    target = battle_unit_get_stats_from_battle_id(g_battle_preview_target_unit_id);
    /* One address register serves both reads, as in the target. */
    selected = &g_battle_active_turn_unit.battle_id;
    action = battle_unit_get_runtime_action_command_by_battle_id(*selected);
    action->actor_id = *selected;
    if (action->ability_id < ABILITY_ID_ITEM_FIRST) {
        main_ability_calculate_pointers_and_type(action->ability_id & ABILITY_ID_MASK, &ability_data, &secondary);
    }
    if (((secondary.range->flags_3 & ABILITY_SECONDARY_FLAG_3_PERSEVERE) && action->ability_id < ABILITY_ID_ITEM_FIRST)
        || action->skillset_id == SKILLSET_ID_EQUIP_CHANGE) {
        g_battle_menu_preview_target_action = 0;
    } else {
        g_thread_call_target = (void (*)(void))action_calculate_projected_effect;
        thread_call_on_main_stack(attacker, target, action);
        g_battle_menu_preview_target_action = &target->action;
    }
    thread_yield();
    thread_yield();
    battle_menu_load_selected_unit();
    battle_menu_display_hovered_unit_stats(&g_battle_menu_attack_caster_stats_display, UNIT_STATS_PANEL_ATTACK_CASTER,
        g_battle_menu_attack_caster_stats_setup, &g_battle_menu_active_turn_banner);
}
