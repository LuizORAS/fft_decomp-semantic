#include "fft/event_require.h"
#include "fft/thread.h"

/* REQUIRE's stand-in for equip_panel_run_equipment_list_thread: yields once and exits. Nothing starts it. */
void require_panel_run_equipment_list_thread(void) {
    thread_yield();
    battle_thread_exit_current();
}
