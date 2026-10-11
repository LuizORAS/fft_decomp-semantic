#include "fft/event_debugchr.h"

/* DEBUGCHR's stand-in for equip_panel_run_equipment_list_thread: yields once and exits. Nothing starts it. */
void debugchr_panel_run_equipment_list_thread(void) {
    battle_thread_yield();
    battle_thread_exit_current();
}
