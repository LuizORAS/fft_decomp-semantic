#include "fft/battle_effect.h"
#include "psx/types.h"

/* 1 when the ability's entry in g_battle_effect_ability_files carries
 * BATTLE_EFFECT_ABILITY_FLAG_ITEM. A negative entry (an ability without an effect
 * file) is complemented before the test. */
s32 battle_effect_is_item_ability(s32 ability_id) {
    s32 entry;
    entry = g_battle_effect_ability_files[ability_id];
    if (entry < 0) {
        entry = ~entry;
    }
    return (entry & BATTLE_EFFECT_ABILITY_FLAG_ITEM) > 0;
}
