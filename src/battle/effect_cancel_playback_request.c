#include "fft/battle_effect.h"

/* Drops a requested effect that has not been released. */
void effect_cancel_playback_request(void) {
    if (g_effect_playback_phase == EFFECT_PLAYBACK_PHASE_REQUESTED) {
        g_effect_playback_phase = EFFECT_PLAYBACK_PHASE_IDLE;
    }
}
