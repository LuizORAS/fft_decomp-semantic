#include "fft/battle_menu.h"
#include "fft/thread.h"

/* Start the fixed worker used by the system-function dispatcher.
 *
 * The dispatcher at 0x80141B0C calls this helper with command-specific
 * values. Thread 4 receives that value as its first parameter after the
 * shared idle-action menu flags are cleared. */
void battle_menu_start_system_function_thread(s32 system_function) {
    battle_menu_clear_entry_selections();
    thread_start(NATIVE_THREAD_SLOT_SYSTEM_FUNCTION, battle_menu_run_system_function_thread);
    thread_set_parameters(NATIVE_THREAD_SLOT_SYSTEM_FUNCTION, system_function, 0, 0);
}
