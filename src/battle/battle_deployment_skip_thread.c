#include "fft/battle.h"
#include "psx/types.h"

/* Stand-in for the deployment screen when the scenario has no first squad to deploy:
 * battle_menu_init_attack_resources_and_threads starts it in the deployment slot instead of ATTACK's
 * attack_deploy_run_screen. One frame later it marks the deployment finished, which step 8 of
 * battle_map_step_setup_sequence waits for, and it exits a frame after that. */
void battle_deployment_skip_thread(void) {
    battle_thread_yield();
    battle_deployment_mark_finished();
    battle_thread_yield();
    battle_thread_exit_current();
}
