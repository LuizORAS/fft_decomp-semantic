#include "fft/battle_action.h"
#include "psx/types.h"

/* rand() when the action is executing; otherwise the midpoint 0x4000, so
 * previews and AI scoring see the average roll. */
s32 battle_formula_get_random_0_7fff(void) {
    if (g_action_state != ACTION_STATE_EXECUTE) {
        return 0x4000;
    }
    return rand();
}
