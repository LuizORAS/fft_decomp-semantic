#include "fft/battle_gfx.h"
#include "psx/types.h"

/* Tint each of the sixteen units by its team (battle_gfx_modify_palette_based_on_team, amount 4); the
 * target selection (battle_target_select_tile) and battle_state_enter_highlight_units_by_team show the
 * teams this way. */
void battle_gfx_tint_all_units_by_team(void) {
    s32 i;

    for (i = 0; i < BATTLE_UNIT_RUNTIME_SLOT_COUNT; i++) {
        battle_gfx_modify_palette_based_on_team(battle_unit_get_by_runtime_id(i), 4);
    }
}
