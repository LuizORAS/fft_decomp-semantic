#include "fft/world.h"
#include "psx/types.h"

/* Ask a thread to stop by setting its third parameter to 1. Used for every
 * panel and menu thread, not only the status banner. */
void world_menu_stop_unit_status_banner_thread(s32 thread_id) {
    world_thread_set_parameters(thread_id, 0, 0, 1);
}
