#include "fft/battle_map.h"
#include "fft/world.h"

/* Thread body: after one yield, mark the asynchronous map-data load
 * complete, yield once more and exit. */
void world_map_data_load_complete_thread(void) {
    world_thread_yield();
    map_mark_data_load_complete();
    world_thread_yield();
    world_thread_exit_current();
}
