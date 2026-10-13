#include "fft/battle_map.h"
#include "fft/thread.h"

/* Mark map-data loading complete after one yield, then exit after another. */
void map_data_load_complete_thread(void) {
    thread_yield();
    map_mark_data_load_complete();
    thread_yield();
    thread_exit_current();
}
