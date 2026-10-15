#include "fft/battle.h"
#include "psx/types.h"

/* Return 1 while battle_effect_update_file_load has work left, else 0: the event script's EffectStart
 * instruction runs it on the main stack once a frame until it returns 0. */
s32 battle_effect_poll_file_load(void) {
    /* The target loads no argument; the parameter is the $a0 value unhandled states return. */
    return battle_effect_update_file_load() != 0;
}
