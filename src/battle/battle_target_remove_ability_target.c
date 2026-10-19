#include "fft/battle_target.h"
#include "psx/types.h"

/* Remove target_battle_id from the ability target list (g_battle_target_ability_targets_list),
 * shift the trailing entries down, and terminate the list with 0xFF. No-op if the id is not
 * present. */
void battle_target_remove_ability_target(s32 target_battle_id) {
    s32 i;

    for (i = 0; i < BATTLE_UNIT_RUNTIME_SLOT_COUNT; i++) {
        if (g_battle_target_ability_targets_list[i] == (target_battle_id & 0xFF)) {
            for (; i < BATTLE_UNIT_RUNTIME_SLOT_COUNT - 1; i++) {
                g_battle_target_ability_targets_list[i] = g_battle_target_ability_targets_list[i + 1];
            }
            g_battle_target_ability_targets_list[BATTLE_UNIT_RUNTIME_SLOT_COUNT - 1] = 0xFF;
            return;
        }
    }
}
