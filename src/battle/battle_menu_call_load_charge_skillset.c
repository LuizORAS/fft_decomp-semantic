#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_menu_load_charge_skillset with scratch CT and turn lists, keeping only the ability
 * ids; battle_menu_load_skillset calls it for the Charge menu. */
s32 battle_menu_call_load_charge_skillset(s32 unit_id, s32 skillset_id, s16* ability_ids) {
    s32 charge_times[4];
    s32 turn_counts[4];
    return battle_menu_load_charge_skillset(unit_id, (u8)skillset_id, ability_ids, (u8*)charge_times, (u8*)turn_counts);
}
