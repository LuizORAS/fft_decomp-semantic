#include "fft/battle.h"

/* Drops a requested effect that has not been released. */
void battle_effect_set_phase_none(void) {
    if (g_battle_effect_phase == BATTLE_EFFECT_PHASE_REQUESTED) {
        g_battle_effect_phase = BATTLE_EFFECT_PHASE_IDLE;
    }
}
