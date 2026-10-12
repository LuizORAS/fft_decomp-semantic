#include "fft/battle_action.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Copy 16 target ids into the current ability's target list. */
void action_store_target_ids(u8* src) {
    s32 i = 0;
    do {
        g_target_ability_targets_list[i] = *src;
        i++;
        src++;
    } while (i < BATTLE_UNIT_MISC_SLOT_COUNT);
}
