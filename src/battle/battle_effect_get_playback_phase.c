#include "fft/battle_effect.h"

/* Return g_battle_effect_playback_phase (effect_playback_phase_e). The linear projectile handler tests it for an
 * effect still waiting to be released. */
s32 battle_effect_get_playback_phase(void) {
    return g_battle_effect_playback_phase;
}
