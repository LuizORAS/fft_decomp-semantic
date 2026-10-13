#include "fft/battle_map.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Thread body: after one yield, mark the asynchronous map-data load
 * complete, yield once more and exit. */
void world_map_data_load_complete_thread(void) {
    thread_yield();
    map_mark_data_load_complete();
    thread_yield();
    world_thread_exit_current();
}
