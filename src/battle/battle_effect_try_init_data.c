#include "fft/battle.h"
#include "psx/types.h"

s32 battle_effect_try_init_data(void) {
    /* The target loads no argument; the parameter is the $a0 value unhandled states return. */
    return battle_effect_init_data() != 0;
}
