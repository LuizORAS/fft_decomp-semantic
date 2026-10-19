#include "fft/battle.h"
#include "fft/thread.h"

/* Confirm the current menu action, recording the auto-battle mode it selects. */
void battle_menu_confirm_auto_battle_selection_thread(void) {
    battle_stats_t* attacker;
    s32 menu;
    world_menu_entry_t* parameter;

    parameter = (world_menu_entry_t*)battle_thread_get_current_parameter_1();
    attacker = battle_unit_get_active_turn_unit_stats();
    menu = g_menu_thread_menu_data[BATTLE_MENU_ID_AUTO_BATTLE].selected_index;
    if (menu == 3) {
        attacker->auto_battle_setting = BATTLE_AUTO_BATTLE_SAVE_FADING_LIFE;
    } else if (menu == 4) {
        attacker->auto_battle_setting = BATTLE_AUTO_BATTLE_RUN_LIKE_A_RABBIT;
    }
    g_battle_menu_set_auto_battle_row_actions = -3;
    battle_menu_confirm_action_silently(parameter);
    thread_exit_current();
}
