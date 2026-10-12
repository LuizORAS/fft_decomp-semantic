#include "fft/battle_effect.h"
#include "psx/types.h"

/* effect_init_data as a 0 or 1 result: the event script's EffectStart
 * instruction calls it on the main stack each frame until it returns 0. */
s32 effect_try_init_data(void) {
    /* The target loads no argument; the parameter is the $a0 value unhandled states return. */
    return effect_init_data() != 0;
}
