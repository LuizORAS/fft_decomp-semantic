#include "fft/world.h"
#include "psx/types.h"

/* The event thread body world_script_start_current_event_thread starts. */
void world_script_run_next_event(void) {
    world_script_load_next_event();
    world_script_execute_event();
}
