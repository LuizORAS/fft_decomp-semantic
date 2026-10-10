#include "fft/battle.h"

/* Releases a requested effect: battle_effect_update_stage starts it on its
 * next frame. */
void battle_effect_play(void) {
    if (g_battle_effect_phase == BATTLE_EFFECT_PHASE_REQUESTED) {
        g_battle_effect_phase = BATTLE_EFFECT_PHASE_INIT_PALETTE;
    }
}
