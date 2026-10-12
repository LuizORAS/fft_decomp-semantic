#include "fft/battle_effect.h"

/* Releases a requested effect: effect_update_stage starts it on its
 * next frame. */
void effect_play(void) {
    if (g_effect_playback_phase == EFFECT_PLAYBACK_PHASE_REQUESTED) {
        g_effect_playback_phase = EFFECT_PLAYBACK_PHASE_INIT_PALETTE;
    }
}
