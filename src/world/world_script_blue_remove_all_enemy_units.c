#include "fft/battle_gfx.h"
#include "fft/battle_unit.h"
#include "fft/wldcore.h"
#include "fft/world.h"

/* Tints blue (red and green modulated by -0x1f) every displayed unit whose
 * initial team is not the player's. */
void world_script_blue_remove_all_enemy_units(void) {
    s32 resolved_id;
    s32 unit_index;
    s32 runtime_id;

    for (runtime_id = 0; runtime_id < BATTLE_UNIT_SLOT_COUNT; runtime_id++) {
        if (battle_unit_has_runtime_id(runtime_id) != 0) {
            unit_index = wldcore_unit_get_battle_id_by_runtime_id(runtime_id);
            if (unit_index != -1
                && (battle_unit_find_stats_by_entd_unit_id(
                        battle_unit_get_stats_from_battle_id(unit_index)->unit_id, &resolved_id)
                           ->initial_team_flags
                       & BATTLE_TEAM_MASK)
                    != BATTLE_TEAM_BLUE
                && resolved_id != -2) {
                battle_gfx_start_event_unit_fade_out(runtime_id, 2, -0x1f, -0x1f, 0);
            }
        }
    }
}
