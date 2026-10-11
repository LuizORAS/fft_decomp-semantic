#include "fft/battle.h"
#include "psx/types.h"

/* Opens the non-active-unit idle action menu for the unit under the cursor;
 * g_battle_status_menu_open switches menu ids 0xf/0x10 to 0x11/0x12. */
void battle_menu_open_non_active_unit_idle_action_menu(void) {
    battle_unit_misc_data_t* misc;
    battle_stats_t* unit;
    s32 menu_id;

    misc = battle_unit_get_selectable_misc_data_at_map_coords(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    if (misc == 0) {
        return;
    }
    battle_state_disable_camera_pan();
    unit = misc->battle_data;
    g_battle_game_state = BATTLE_GAME_STATE_IDLING_ACTION_MENUS;
    battle_unit_start_view_thread(UNIT_VIEW_MODE_UNIT_SUMMARY, unit->misc_unit_id, 0);
    menu_id = battle_menu_get_id_based_on_mount_moveable_actable(misc->battle_data->misc_unit_id);
    if (g_battle_status_menu_open != 0) {
        if (menu_id == BATTLE_MENU_ID_PLAYER_UNIT) {
            menu_id = BATTLE_MENU_ID_PLAYER_UNIT_INSPECT;
        } else if (menu_id == BATTLE_MENU_ID_OTHER_UNIT) {
            menu_id = BATTLE_MENU_ID_OTHER_UNIT_INSPECT;
        }
    }
    battle_menu_build_idle_action_menu(menu_id);
}
