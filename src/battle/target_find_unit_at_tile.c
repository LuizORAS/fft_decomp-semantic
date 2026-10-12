#include "fft/battle_target.h"

/* The unit on a tile: battle_unit_find_at_tile with no status filter. */
s32 target_find_unit_at_tile(s32 x, s32 y, s32 level) {
    return battle_unit_find_at_tile(x, y, level, 0);
}
