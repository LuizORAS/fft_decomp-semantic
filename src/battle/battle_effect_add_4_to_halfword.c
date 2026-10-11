#include "fft/battle.h"
#include "psx/types.h"

/* Nothing in the game calls it (see battle_effect_add_2_to_halfword). */
void battle_effect_add_4_to_halfword(u16* value) {
    *value += 4;
}
