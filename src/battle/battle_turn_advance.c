#include "fft/battle.h"
#include "psx/types.h"

/* Go on to the next turn event at 60 fps, unless a Deep Dungeon map load starts first: take
 * it from the clock and make its unit the casting unit. When the player pressed Triangle during
 * an AI unit's turn, the next AI turn first opens the status menu at the top of the AT list;
 * otherwise, unless battle_menu_init_action_menu starts an ending or scenario event, enter
 * CHANGE_TURN. A unit with no battle record returns to FREE_CURSOR. */
void battle_turn_advance(void) {
    battle_unit_misc_data_t* unit;

    if (battle_map_try_start_deep_dungeon_mesh_load() != 0) {
        return;
    }
    battle_turn_clear_at_list_index();
    g_battle_state_vsync_interval = 1;
    battle_turn_take_next_event();
    unit = battle_unit_get_source_misc_data();
    if (unit == 0) {
        return;
    }
    g_battle_casting_unit_id = unit->unit_id;
    battle_unit_store_entd_flags_without_control_flag(unit);
    if (unit->battle_data != 0) {
        battle_menu_reset_unit_record(unit->battle_data->misc_unit_id);
        battle_unit_store_entd_flags_without_control_flag(unit);
        if (g_battle_menu_status_requested != 0 && !(unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED)
            && g_battle_turn_event == BATTLE_TURN_EVENT_UNIT_READY) {
            battle_unit_update_display_by_misc_id(unit->unit_id);
            main_sound_play_sfx(0x71);
            battle_turn_start_at_list_browse();
            battle_turn_show_next_at_entry();
            battle_menu_set_next_script_action_menus();
            return;
        }
        g_battle_action_post_action = 0;
        if (g_battle_turn_event == BATTLE_TURN_EVENT_UNIT_READY || g_battle_turn_event == BATTLE_TURN_EVENT_NONE) {
            battle_unit_update_display_by_misc_id(unit->unit_id);
        }
        if (battle_menu_init_action_menu(unit) == 0) {
            battle_menu_store_unit_names_and_event_block_data(1, 0xff, 0xff);
            g_battle_game_state = BATTLE_GAME_STATE_CHANGE_TURN;
            battle_ai_init_selected_action();
            g_battle_menu_status_enabled = 1;
        } else {
            battle_unit_update_display_by_misc_id(unit->unit_id);
        }
        g_battle_menu_status_requested = 0;
    } else {
        battle_state_enter_free_cursor();
    }
}
