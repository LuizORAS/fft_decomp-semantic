#include "fft/battle_effect.h"

/* Nothing in the game calls it. */
s32 effect_get_prim_buffer_offset(void) {
    return g_effect_prim_buffer_offset;
}
