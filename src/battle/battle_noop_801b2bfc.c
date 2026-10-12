#include "fft/battle_effect.h"

/* Secondary effect function 0x0a in g_battle_effect_secondary_handlers: an empty
 * handler. It sets no result, although battle_effect_update_secondary_effects
 * tests one to decide whether the effect ends. */
s32 battle_noop_801b2bfc(void) {
}
