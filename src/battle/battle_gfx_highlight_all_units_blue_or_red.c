#include "fft/battle_gfx.h"
#include "fft/battle_map.h"

/* Starts the repeating tint that marks the candidates of an auto-battle target
 * choice: red on every unit of another team than the casting unit's for
 * BATTLE_AI_COMMAND_AUTOBATTLE_TARGETED_SELECT, blue on every unit of its own
 * team for BATTLE_AI_COMMAND_AUTOBATTLE_RECOVERY_SELECT. Other modes tint
 * nothing. */
void battle_gfx_highlight_all_units_blue_or_red(s32 mode) {
    battle_unit_misc_data_t* caster;
    battle_unit_misc_data_t* unit;
    s32 i;

    caster = battle_unit_get_casting_misc_data();
    for (i = 0; i < BATTLE_UNIT_MISC_SLOT_COUNT; i++) {
        unit = battle_unit_get_misc_data_by_misc_id(i);
        if (unit == 0) {
            continue;
        }
        if (unit->battle_data == 0) {
            continue;
        }
        switch (mode) {
        case BATTLE_AI_COMMAND_AUTOBATTLE_TARGETED_SELECT:
            if ((unit->battle_data->team_flags & BATTLE_TEAM_MASK)
                != (caster->battle_data->team_flags & BATTLE_TEAM_MASK)) {
                battle_map_modify_palette(MAP_COLOR_MODE_BASE_REPEAT, 4, MAP_CLUT_ROW_UNITS, i, 0, 0x10, 0, 0);
            }
            continue;
        case BATTLE_AI_COMMAND_AUTOBATTLE_RECOVERY_SELECT:
            if ((unit->battle_data->team_flags & BATTLE_TEAM_MASK)
                == (caster->battle_data->team_flags & BATTLE_TEAM_MASK)) {
                battle_map_modify_palette(MAP_COLOR_MODE_BASE_REPEAT, 4, MAP_CLUT_ROW_UNITS, i, 0, 0, 0, 0x18);
            }
            continue;
        }
    }
}
