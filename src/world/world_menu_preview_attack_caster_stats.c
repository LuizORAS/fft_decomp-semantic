#include "fft/battle_action.h"
#include "fft/battle_unit.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/*
 * Attack-preview caster panel, WORLD copy of
 * battle_menu_preview_attack_caster_stats: stamp the selected unit's battle id
 * into its pending action, run the preview calculation on the main
 * stack unless the ability's secondary flag 0x04 is set or the skillset
 * changes equipment, and publish the target's action results through
 * g_world_menu_preview_target_action (0 when skipped).
 *
 * For an item id the ability pointers are never set, yet secondary_data.range->flags_3
 * is still read before the id is tested.
 */
void world_menu_preview_attack_caster_stats(void) {
    battle_stats_t* stored;
    battle_stats_t* target;
    action_command_t* action;
    ability_data_t* ability_data;
    main_ability_secondary_pointer_t secondary_data;
    s16 ability_id;
    s16* stored_unit;

    stored = world_unit_get_battle_stats_for_stored();
    target = battle_unit_get_stats_from_battle_id(g_world_unit_comparison_battle_id);
    /* Local pointer keeps the stored-unit address in one base register. */
    stored_unit = &g_world_active_unit_data.battle_id;
    action = battle_unit_get_runtime_action_command_by_battle_id(*stored_unit);
    action->actor_id = *stored_unit;
    ability_id = action->ability_id;
    if (action->ability_id < ABILITY_ID_ITEM_FIRST) {
        main_ability_calculate_pointers_and_type(ability_id & ABILITY_ID_MASK, &ability_data, &secondary_data);
    }
    if (((secondary_data.range->flags_3 & 4) && action->ability_id < ABILITY_ID_ITEM_FIRST)
        || action->skillset_id == SKILLSET_ID_EQUIP_CHANGE) {
        g_world_menu_preview_target_action = 0;
    } else {
        g_thread_call_target = (void (*)(void))action_calculate_projected_effect;
        thread_call_on_main_stack(stored, target, action);
        g_world_menu_preview_target_action = &target->action;
    }
    thread_yield();
    thread_yield();
    world_menu_load_selected_unit();
    world_menu_display_hovered_unit_stats(&g_world_menu_attack_caster_stats_display, UNIT_STATS_PANEL_ATTACK_CASTER,
        g_world_menu_attack_caster_stats_setup, &g_world_unit_selected_status_billboard);
}
