#include "fft/battle_map.h"
#include "fft/thread.h"

/* Stand-in for the deployment screen when the scenario has no first squad to deploy:
 * battle_menu_init_attack_resources_and_threads starts it in the deployment slot instead of ATTACK's
 * attack_deploy_run_screen. One frame later it marks the deployment finished, which step 8 of
 * battle_map_step_setup_sequence waits for, and it exits a frame after that. WORLD's copy's only
 * starter, world_menu_init_attack_resources_and_threads, has no caller (QUIRKS.md). */
void deployment_skip_thread(void) {
    thread_yield();
    battle_deployment_mark_finished();
    thread_yield();
    thread_exit_current();
}
