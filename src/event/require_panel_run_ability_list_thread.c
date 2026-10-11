#include "fft/event_require.h"

/* REQUIRE's stand-in for equip_panel_run_ability_list_thread: yields once and exits. Nothing starts it. */
void require_panel_run_ability_list_thread(void) {
    battle_thread_yield();
    battle_thread_exit_current();
}
