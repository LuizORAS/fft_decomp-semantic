#include "fft/battle.h"
#include "psx/types.h"

/* Tint each of the sixteen units by its team (battle_gfx_modify_palette_based_on_team, amount 4); the
 * target selection (battle_target_select_tile) and battle_state_enter_highlight_units_by_team show the
 * teams this way. */
void battle_gfx_tint_all_units_by_team(void) {
    s32 i = 0;
    do {
        battle_gfx_modify_palette_based_on_team(battle_unit_get_misc_data_by_misc_id(i), 4);
        i++;
    } while (i < 0x10);
}
