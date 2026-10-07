#include "fft/battle.h"
#include "psx/types.h"

/* Load the skillset's learned Charge abilities (Charge +1 to +20) with their CT and turn counts
 * into the caller's lists (battle_menu_get_unit_skillset_ability_data); the MP-cost and flag lists are
 * scratch. */
s32 battle_menu_load_charge_skillset(s32 unit_id, s32 skillset, s16* abilities, u8* ability_ct, u8* turns) {
    s32 mp_costs[4];
    s32 ability_flags[4];
    return battle_menu_get_unit_skillset_ability_data(
        unit_id, skillset, abilities, (u8*)mp_costs, ability_ct, 0, (u8*)ability_flags, turns);
}
