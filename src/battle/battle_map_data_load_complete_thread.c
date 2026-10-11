#include "fft/battle.h"
#include "psx/types.h"

/* Thread that battle_menu_init_attack_resources_and_threads starts in the
 * deployment slot: one frame later it sets g_battle_map_data_load_complete, which
 * step 8 of the expanded map init sequence waits for, and it exits a frame after
 * that. */
void battle_map_data_load_complete_thread(void) {
    battle_thread_yield();
    battle_map_mark_data_load_complete();
    battle_thread_yield();
    battle_thread_exit_current();
}
