#include "fft/battle.h"

/* Tint a unit's palette by its team: blue, red, green or light blue.
 *
 * `amount` passes through to battle_map_modify_palette. */
void battle_gfx_modify_palette_based_on_team(battle_unit_misc_data_t* unit, s32 amount) {
    if (unit == 0) {
        return;
    }
    if (unit->battle_data == 0) {
        return;
    }
    switch ((unit->battle_data->team_flags & BATTLE_TEAM_MASK) >> 4) {
    case BATTLE_TEAM_BLUE >> 4:
        battle_map_modify_palette(MAP_COLOR_MODE_BASE_REPEAT, amount, MAP_CLUT_ROW_UNITS, unit->unit_id, 0, 0, 0, 0x18);
        break;
    case BATTLE_TEAM_RED >> 4:
        battle_map_modify_palette(MAP_COLOR_MODE_BASE_REPEAT, amount, MAP_CLUT_ROW_UNITS, unit->unit_id, 0, 0x10, 0, 0);
        break;
    case BATTLE_TEAM_GREEN >> 4:
        battle_map_modify_palette(MAP_COLOR_MODE_BASE_REPEAT, amount, MAP_CLUT_ROW_UNITS, unit->unit_id, 0, 0, 0x10, 0);
        break;
    default:
        battle_map_modify_palette(
            MAP_COLOR_MODE_BASE_REPEAT, amount, MAP_CLUT_ROW_UNITS, unit->unit_id, 0, 0, 0x10, 0x10);
        break;
    }
}
