#include "fft/battle.h"

/* Start the system-function thread (battle_menu_run_system_function_thread) in slot 4 for table
 * entry `system_function`, after clearing the remembered rows of the idle action menus. */
void battle_menu_start_system_function_thread(s32 system_function) {
    battle_menu_clear_entry_selections();
    battle_thread_start(NATIVE_THREAD_SLOT_SYSTEM_FUNCTION, battle_menu_run_system_function_thread);
    battle_thread_set_parameters(NATIVE_THREAD_SLOT_SYSTEM_FUNCTION, system_function, 0, 0);
}
