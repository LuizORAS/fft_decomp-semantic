#include "fft/battle.h"

void battle_effect_play(void) {
    if (g_battle_effect_phase == BATTLE_EFFECT_PHASE_REQUESTED) {
        g_battle_effect_phase = BATTLE_EFFECT_PHASE_INIT_PALETTE;
    }
}
