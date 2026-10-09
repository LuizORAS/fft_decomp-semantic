#include "fft/battle.h"
#include "psx/types.h"

s32 battle_unit_add_unique_entd_spritesheet(s32 mode, s16 value, s32 limit) {
    s32 idx;
    s32 i;

    if (mode == 0xFF) {
        g_battle_unit_entd_spritesheet_count = 0;
        return 0;
    }
    if (mode == 0) {
        g_battle_unit_entd_spritesheet_count = 1;
        g_battle_unit_entd_spritesheet_ids[0] = value;
        return 0;
    }
    idx = 0xFF;
    for (i = 0; i < g_battle_unit_entd_spritesheet_count; i++) {
        if (g_battle_unit_entd_spritesheet_ids[i] == value) {
            idx = i;
            break;
        }
    }
    if (idx == 0xFF) {
        idx = g_battle_unit_entd_spritesheet_count;
        g_battle_unit_entd_spritesheet_count = idx + 1;
        g_battle_unit_entd_spritesheet_ids[idx] = value;
    }
    return idx >= limit;
}
