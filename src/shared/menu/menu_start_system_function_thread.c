#include "fft/battle_menu.h"
#include "fft/menu.h"
#include "fft/thread.h"

/* Start the system-function thread (battle_menu_run_system_function_thread) in slot 4 for table
 * entry `system_function`, after clearing the remembered rows of the idle action menus. WORLD compiles
 * this text too. */
void menu_start_system_function_thread(s32 system_function) {
    battle_menu_clear_entry_selections();
    thread_start(THREAD_SLOT_SYSTEM_FUNCTION, battle_menu_run_system_function_thread);
    thread_set_parameters(THREAD_SLOT_SYSTEM_FUNCTION, system_function, 0, 0);
}
