#include "fft/battle_target.h"
#include "psx/types.h"

/* Copy the 16-entry ability target list (g_battle_target_ability_targets_list) into out and
 * return how many entries hold a unit (any value but 0xff). */
s32 battle_target_copy_ability_targets(u8* out) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < BATTLE_UNIT_RUNTIME_SLOT_COUNT; i++) {
        u8 target_id = g_battle_target_ability_targets_list[i];
        *out++ = target_id;
        if (target_id != 0xFF) {
            count++;
        }
    }
    return count;
}
