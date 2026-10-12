#include "fft/battle_effect.h"
#include "psx/types.h"

/* Nothing in the game calls it (see effect_add_2_to_halfword). */
void effect_add_8_to_halfword(u16* value) {
    *value += 8;
}
