#include "psx/types.h"

/* WORLD's copy of battle_script_clamp_s32_to_range: clamp `*value` into min..max in place. */
void world_script_clamp_s32_to_range(s32* value, s32 min, s32 max) {
    if (*value < min) {
        *value = min;
        return;
    }
    if (*value > max) {
        *value = max;
    }
}
