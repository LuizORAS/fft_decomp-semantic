#include "fft/battle.h"

/* Secondary effect function 0x07 in g_battle_effect_secondary_handlers: an empty
 * handler. It sets no result, although battle_effect_update_secondary_effects
 * tests one to decide whether the effect ends. */
s32 battle_noop_801b2968(void) {
}
