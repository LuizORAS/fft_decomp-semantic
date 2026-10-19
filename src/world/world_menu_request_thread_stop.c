#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Ask thread `thread_id` to stop by setting its third parameter to 1; every menu and panel thread
 * polls it. */
void world_menu_request_thread_stop(s32 thread_id) {
    thread_set_parameters(thread_id, 0, 0, 1);
}
