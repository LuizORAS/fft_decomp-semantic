#include "fft/battle_map.h"
#include "fft/battle_thread.h"
#include "fft/thread.h"

/* Thread that battle_menu_init_attack_resources_and_threads starts in the
 * deployment slot: one frame later it sets g_map_data_load_complete, which
 * step 8 of the expanded map init sequence waits for, and it exits a frame after
 * that. */
void map_data_load_complete_thread(void) {
    thread_yield();
    map_mark_data_load_complete();
    thread_yield();
    battle_thread_exit_current();
}
