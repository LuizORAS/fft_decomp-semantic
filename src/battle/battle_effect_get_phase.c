#include "fft/battle.h"

/* Return g_battle_effect_phase (battle_effect_phase_e). The linear projectile handler tests it for an
 * effect still waiting to be released. */
s32 battle_effect_get_phase(void) {
    return g_battle_effect_phase;
}
