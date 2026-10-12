#include "fft/battle.h"
#include "fft/wldcore.h"
#include "fft/world.h"

/* BlueRemoveUnit (EVENT_OPCODE_BLUE_REMOVE_UNIT): tint blue every non-player unit whose ENTD unit id
 * is unit_id (red and green modulated by -0x1f).
 *
 * The team test reads the current team flags, not the initial ones. */
void world_script_blue_remove_unit(s32 unit_id) {
    s32 resolved_id;
    s32 unit_index;
    s32 misc_id;
    battle_stats_t* unit;

    misc_id = 0;
    do {
        if (battle_unit_has_misc_id(misc_id) != 0) {
            unit_index = wldcore_unit_get_id_from_misc_id(misc_id);
            if (unit_index != -1) {
                unit = find_unit_by_id(battle_unit_get_stats_from_battle_id(unit_index)->unit_id, &resolved_id);
                if ((unit->team_flags & BATTLE_TEAM_MASK) != BATTLE_TEAM_BLUE && resolved_id != -2
                    && unit->unit_id == unit_id) {
                    battle_gfx_init_misc_unit_palette_modulation(misc_id, 2, -0x1f, -0x1f, 0);
                }
            }
        }
        misc_id++;
    } while (misc_id < BATTLE_UNIT_SLOT_COUNT);
}
