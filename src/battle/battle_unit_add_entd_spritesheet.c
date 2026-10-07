#include "fft/battle.h"
#include "psx/types.h"

/* Track the distinct spritesheets of the encounter's ENTD units (battle_load_entd_units): mode 0xff
 * empties the list, mode 0 (the first unit) starts it with value, and any other mode adds value when it is
 * new. Returns 1 when value's place in the list is at or past limit (the spritesheet slots the player's
 * deployment leaves), which drops the unit. */
s32 battle_unit_add_entd_spritesheet(s32 mode, s16 value, s32 limit) {
    s32 idx;
    s32 i;

    if (mode == 0xFF) {
        g_battle_unit_entd_spritesheet_count = 0;
        return 0;
    }
    if (mode == 0) {
        g_battle_unit_entd_spritesheet_count = 1;
        g_battle_unit_entd_spritesheets[0] = value;
        return 0;
    }
    idx = 0xFF;
    for (i = 0; i < g_battle_unit_entd_spritesheet_count; i++) {
        if (g_battle_unit_entd_spritesheets[i] == value) {
            idx = i;
            break;
        }
    }
    if (idx == 0xFF) {
        idx = g_battle_unit_entd_spritesheet_count;
        g_battle_unit_entd_spritesheet_count = idx + 1;
        g_battle_unit_entd_spritesheets[idx] = value;
    }
    return idx >= limit;
}
