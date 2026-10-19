#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "fft/battle_menu.h"

/* Go on to the next turn event at 60 fps, unless a Deep Dungeon map load starts first: take
 * it from the clock and make its unit the casting unit. When the player pressed Triangle during
 * an AI unit's turn, the next AI turn first opens the status menu at the top of the AT list;
 * otherwise, unless battle_state_try_enter_scenario_event starts an ending or scenario event, enter
 * CHANGE_TURN. A unit with no battle record returns to FREE_CURSOR. */
void battle_turn_advance(void) {
    battle_unit_t* unit;

    if (battle_map_try_start_deep_dungeon_mesh_load() != 0) {
        return;
    }
    battle_turn_clear_at_list_index();
    g_main_system_vsync_interval = 1;
    battle_turn_take_next_event();
    unit = battle_unit_get_source();
    if (unit == 0) {
        return;
    }
    g_battle_casting_runtime_id = unit->runtime_id;
    battle_unit_store_entd_flags_without_control_flag(unit);
    if (unit->stats != 0) {
        battle_menu_reset_unit_record(unit->stats->battle_id);
        battle_unit_store_entd_flags_without_control_flag(unit);
        if (g_battle_menu_status_requested != 0 && !(unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED)
            && g_battle_turn_event == BATTLE_TURN_EVENT_UNIT_READY) {
            battle_unit_update_display_by_runtime_id(unit->runtime_id);
            main_sound_play_sfx(MAIN_SFX_STATUS_MENU_REQUEST);
            battle_turn_start_at_list_browse();
            battle_turn_show_next_at_entry();
            battle_state_enter_open_action_menus();
            return;
        }
        g_battle_action_post_action = 0;
        if (g_battle_turn_event == BATTLE_TURN_EVENT_UNIT_READY || g_battle_turn_event == BATTLE_TURN_EVENT_NONE) {
            battle_unit_update_display_by_runtime_id(unit->runtime_id);
        }
        if (battle_state_try_enter_scenario_event(unit) == 0) {
            battle_unit_start_view_thread(UNIT_VIEW_MODE_TILE_INFO, 0xff, 0xff);
            g_battle_game_state = BATTLE_GAME_STATE_CHANGE_TURN;
            battle_ai_init_selected_action();
            g_battle_menu_status_enabled = 1;
        } else {
            battle_unit_update_display_by_runtime_id(unit->runtime_id);
        }
        g_battle_menu_status_requested = 0;
    } else {
        battle_state_enter_free_cursor();
    }
}
