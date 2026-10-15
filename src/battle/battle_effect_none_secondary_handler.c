#include "psx/types.h"

/* Secondary effect function 0x00 of g_battle_effect_secondary_handlers, which the NONE animation names:
 * its result of 0 makes battle_effect_update_secondary_effects end the effect at once. */
s32 battle_effect_none_secondary_handler(void) {
    return 0;
}
