#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/* WORLD's copy of battle_deployment_skip_thread. Its only starter,
 * world_menu_init_attack_resources_and_threads, has no caller (QUIRKS.md). */
void world_deployment_skip_thread(void) {
    world_thread_yield();
    battle_deployment_mark_finished();
    world_thread_yield();
    world_thread_exit_current();
}
