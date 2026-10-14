#include "fft/battle.h"
#include "psx/types.h"

/* Set g_battle_deployment_finished: ATTACK's attack_deploy_run_screen calls it once the deployed
 * units' coordinates are written, battle_deployment_skip_thread when the battle has no deployment.
 * battle_state_run_battle_setup clears it and step 8 of battle_map_step_setup_sequence waits for it. */
void battle_deployment_mark_finished(void) {
    g_battle_deployment_finished = 1;
}
