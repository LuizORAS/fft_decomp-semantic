#include "fft/script.h"

/* A second copy of script_run_next_event. Nothing starts it; WORLD has the same pair. */
void script_run_next_event_unused(void) {
    script_load_next_event();
    script_execute_event();
}
