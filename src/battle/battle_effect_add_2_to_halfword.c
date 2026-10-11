#include "fft/battle.h"
#include "psx/types.h"

/* Nothing in the game calls this or its three siblings for 4, 6 and 8. */
void battle_effect_add_2_to_halfword(u16* value) {
    *value += 2;
}
