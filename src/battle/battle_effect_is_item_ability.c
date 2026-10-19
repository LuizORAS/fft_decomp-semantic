#include "fft/battle_effect.h"
#include "psx/types.h"

/* Return 1 when the ability's entry in g_battle_effect_ability_files carries
 * BATTLE_EFFECT_ABILITY_FLAG_ITEM, testing the complement of a negative entry (an ability without an effect
 * file). battle_action_play_ability_effect releases an item ability's effect only once the unit holds up the
 * item. */
s32 battle_effect_is_item_ability(s32 ability_id) {
    s32 entry;
    entry = g_battle_effect_ability_files[ability_id];
    if (entry < 0) {
        entry = ~entry;
    }
    return (entry & BATTLE_EFFECT_ABILITY_FLAG_ITEM) > 0;
}
