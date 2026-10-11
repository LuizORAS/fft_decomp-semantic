#include "fft/battle.h"

/* A second copy of battle_script_run_next_event. Nothing starts it; WORLD has the same pair. */
void battle_script_run_next_event_unused(void) {
    battle_script_load_next_event();
    battle_script_execute_event();
}
