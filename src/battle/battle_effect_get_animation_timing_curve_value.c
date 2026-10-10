#include "fft/battle.h"
#include "psx/types.h"

s32 battle_effect_get_animation_timing_curve_value(battle_effect_timing_entry_t* entry) {
    s16 packed_index;
    s32 offset;
    s32 packed_value;

    packed_index = entry->timeline_frame;
    offset = packed_index / 2;
    packed_value = g_battle_effect_timing_curves->child_timeline[offset];
    if (packed_index & 1) {
        packed_value >>= 4;
    }
    return packed_value & 0xf;
}
