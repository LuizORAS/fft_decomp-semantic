#include "fft/battle_effect.h"

/* Releases a requested effect: battle_effect_update_stage starts it on its
 * next frame. */
void battle_effect_play(void) {
    if (g_battle_effect_playback_phase == BATTLE_EFFECT_PLAYBACK_PHASE_REQUESTED) {
        g_battle_effect_playback_phase = BATTLE_EFFECT_PLAYBACK_PHASE_INIT_PALETTE;
    }
}
