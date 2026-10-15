#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_effect_reset_frame_data_pointer for g_battle_effect_animation_sequences, whose start
 * pointer nothing writes either. */
void battle_effect_reset_animation_sequences_pointer(void) {
    g_battle_effect_animation_sequences = g_battle_effect_animation_sequences_start;
}
