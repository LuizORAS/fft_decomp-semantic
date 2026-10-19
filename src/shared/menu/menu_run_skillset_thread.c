#include "fft/menu.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Thread of the Act row (SKILLSET_LIST): check that the unit may act, build its skillset list on the
 * main stack and run it as an icon list from the row the unit last chose, saving the row. A monster's
 * single skillset skips the list: its ability list opens at once (on a menu restore the thread only
 * waits for it). WORLD compiles this text too.
 *
 * The completion value is materialised before the branch (`li v1,1` in the beqz delay slot, ahead of
 * the 0xFC argument) and stored through a pointer to the thread context, which keeps the row loads
 * after that store. */
void menu_run_skillset_thread(void) {
    s32 param_value;
    battle_menu_unit_selection_record_t* row;
    s32* param;

    world_menu_check_action_restrictions();
    g_thread_call_target = world_menu_build_skillset_entries;
    thread_call_on_main_stack();
    param_value = 1;
    if (g_menu_skillset_build_result != 0) {
        g_menu_thread_menu_data[BATTLE_MENU_ID_SKILLSET_LIST].selected_index = 0;
        if (g_menu_restore_pending == 0) {
            thread_start(g_thread_current_id - 1, world_menu_select_unit_action_slots_thread);
            thread_set_parameters(
                g_thread_current_id - 1, (s32)&g_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_LIST], 0, 0);
        } else {
            thread_wait_frames(2);
        }
        for (;;) {
            thread_yield();
            if (world_thread_is_running_asm(g_thread_current_id - 1) == 0) {
                thread_exit_current();
            }
        }
    }
    param = &g_thread_contexts[g_thread_current_id].function_parameter_2;
    *param = param_value;
    row = &g_world_menu_unit_selection_records[g_menu_selected_unit.battle_id];
    g_menu_thread_menu_data[BATTLE_MENU_ID_SKILLSET_LIST].selected_index = row->skillset_index;
    g_world_menu_restore_depth = 1;
    script_pulse_tutorial_wait_value(0xFC);
    world_menu_run_icon_selection_loop();
    row->skillset_index = g_menu_thread_menu_data[BATTLE_MENU_ID_SKILLSET_LIST].selected_index;
    thread_exit_current();
}
