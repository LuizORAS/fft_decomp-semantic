#include "fft/battle.h"
#include "psx/types.h"

/* A random number from 0 to 0x7fff (rand) while executing; outside that (AI simulation, preview) the
 * middle value 0x4000, so estimates use an average roll. */
s32 battle_formula_get_random_0_7fff(void) {
    if (g_battle_action_state != BATTLE_ACTION_STATE_EXECUTE) {
        return 0x4000;
    }
    return rand();
}
