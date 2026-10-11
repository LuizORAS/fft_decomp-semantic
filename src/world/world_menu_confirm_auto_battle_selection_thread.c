#include "fft/world.h"
#include "psx/types.h"

/* Applies the auto-battle choice of the menu selection (3 or 4) to the stored unit and confirms the
 * thread's menu entry without a sound. */
void world_menu_confirm_auto_battle_selection_thread(void) {
    world_menu_entry_t* entry = world_thread_get_current_parameter_1();
    battle_stats_t* unit = world_unit_get_active_turn_unit_stats();
    world_menu_thread_data_t* menu = (world_menu_thread_data_t*)g_world_menu_thread_menu_data;

    if (menu->selection == 3) {
        unit->auto_battle_setting = BATTLE_AUTO_BATTLE_SAVE_FADING_LIFE;
    } else if (menu->selection == 4) {
        unit->auto_battle_setting = BATTLE_AUTO_BATTLE_RUN_LIKE_A_RABBIT;
    }
    g_world_menu_set_auto_battle_row_actions = -3;
    world_menu_confirm_action_silently(entry);
    world_thread_exit_current();
}
