#include "fft/battle_effect.h"

/* Nothing in the game calls it. */
s32 battle_effect_get_prim_buffer_offset(void) {
    return g_battle_effect_prim_buffer_offset;
}
