#include "fft/battle.h"

/* The unit standing at (x, y, level), or -1 (battle_unit_find_at_tile with no filter). */
s32 battle_target_get_unit_at_tile(s32 x, s32 y, s32 level) {
    return battle_unit_find_at_tile(x, y, level, 0);
}
