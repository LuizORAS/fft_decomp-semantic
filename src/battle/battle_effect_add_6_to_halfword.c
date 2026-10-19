#include "fft/battle_effect.h"
#include "psx/types.h"

/* Nothing in the game calls it (see battle_effect_add_2_to_halfword). */
void battle_effect_add_6_to_halfword(u16* value) {
    *value += 6;
}
