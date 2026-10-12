#include "fft/world.h"

/* A second copy of world_script_run_next_event. Nothing starts it; BATTLE has the same pair. */
void world_script_run_next_event_unused(void) {
    world_script_load_next_event();
    world_script_execute_event();
}
