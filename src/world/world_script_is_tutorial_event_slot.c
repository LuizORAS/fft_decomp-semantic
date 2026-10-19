#include "fft/world.h"
#include "psx/types.h"

/* WORLD's stand-in for script_is_tutorial_event_slot: it always returns 0. */
s32 world_script_is_tutorial_event_slot(void) {
    return 0;
}
