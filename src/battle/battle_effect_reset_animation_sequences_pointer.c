#include "fft/battle.h"
#include "psx/types.h"

void battle_effect_reset_animation_sequences_pointer(void) {
    g_battle_effect_animation_sequences = g_battle_effect_animation_sequences_start;
}
