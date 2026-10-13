#include "fft/battle_gfx.h"
#include "fft/battle_unit.h"
#include "fft/wldcore.h"
#include "fft/world.h"

/* BlueRemoveUnit (event instruction 0x99): tints every non-player unit whose
 * ENTD unit id is unit_id blue (red and green modulated by -0x1f).
 *
 * The team test reads the current team flags, not the initial ones. */
void world_script_blue_remove_unit(s32 unit_id) {
    s32 resolved_id;
    s32 unit_index;
    s32 runtime_id;
    battle_stats_t* unit;

    for (runtime_id = 0; runtime_id < BATTLE_UNIT_SLOT_COUNT; runtime_id++) {
        if (battle_unit_has_runtime_id(runtime_id) != 0) {
            unit_index = wldcore_unit_get_battle_id_by_runtime_id(runtime_id);
            if (unit_index != -1) {
                unit = battle_unit_find_stats_by_entd_unit_id(
                    battle_unit_get_stats_from_battle_id(unit_index)->unit_id, &resolved_id);
                if ((unit->team_flags & BATTLE_TEAM_MASK) != BATTLE_TEAM_BLUE && resolved_id != -2
                    && unit->unit_id == unit_id) {
                    battle_gfx_start_event_unit_fade_out(runtime_id, 2, -0x1f, -0x1f, 0);
                }
            }
        }
    }
}
