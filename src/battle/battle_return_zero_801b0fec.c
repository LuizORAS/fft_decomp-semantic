#include "fft/battle.h"
#include "psx/types.h"

/* Secondary effect function 0x00 in g_battle_effect_secondary_handlers: its
 * result of 0 makes battle_effect_update_secondary_effects end the effect at
 * once. */
s32 battle_return_zero_801b0fec(void) {
    return 0;
}
