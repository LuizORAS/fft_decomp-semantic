#include "fft/battle.h"
#include "psx/types.h"

/* Copy the 16-entry ability target list (g_battle_target_ability_targets_list) into `out` and
 * return how many entries hold a unit. */
s32 battle_target_copy_ability_targets(u8* out) {
    s32 count = 0;
    s32 i = 0;
    do {
        u8 target_id = g_battle_target_ability_targets_list[i];
        *out++ = target_id;
        if (target_id != 0xFF) {
            count++;
        }
        i++;
    } while (i < 0x10);
    return count;
}
