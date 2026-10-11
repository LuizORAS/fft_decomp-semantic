#include "fft/battle.h"
#include "psx/types.h"

/* Copy 16 target ids into the current ability's target list. */
void battle_action_store_target_ids(u8* src) {
    s32 i = 0;
    do {
        g_battle_target_ability_targets_list[i] = *src;
        i++;
        src++;
    } while (i < BATTLE_UNIT_MISC_SLOT_COUNT);
}
