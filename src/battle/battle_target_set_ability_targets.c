#include "fft/battle.h"
#include "psx/types.h"

/* Copy a 16-entry target list into g_battle_target_ability_targets_list, the list
 * battle_target_copy_ability_targets reads back. */
void battle_target_set_ability_targets(u8* src) {
    s32 i = 0;
    do {
        g_battle_target_ability_targets_list[i] = *src;
        i++;
        src++;
    } while (i < 16);
}
