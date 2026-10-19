#include "fft/thread.h"
#include "fft/world.h"

/* WORLD twin of battle_menu_confirm_auto_battle_selection_thread. */
void world_menu_confirm_auto_battle_selection_thread(void) {
    world_menu_entry_t* entry = (world_menu_entry_t*)world_thread_get_current_parameter_1();
    battle_stats_t* unit = world_unit_get_selected_unit_stats();
    world_menu_entry_t* menu = g_menu_thread_menu_data;

    if (menu[1].selected_index == 3) {
        unit->auto_battle_setting = BATTLE_AUTO_BATTLE_SAVE_FADING_LIFE;
    } else if (menu[1].selected_index == 4) {
        unit->auto_battle_setting = BATTLE_AUTO_BATTLE_RUN_LIKE_A_RABBIT;
    }
    g_world_menu_set_auto_battle_row_actions = -3;
    world_menu_confirm_action_silently(entry);
    thread_exit_current();
}
