#include "fft/battle.h"

/* Tints one unit's two CLUTs through battle_map_modify_palette; `preset_color` is
 * the MAP_COLOR_MODE_* command and `time` its amount.
 *
 * The unit's CLUT of MAP_CLUT_ROW_UNITS takes the offsets as given. Its CLUT of
 * MAP_CLUT_ROW_UNIT_SHADOWS takes the same offsets while the unit stands on a
 * tile with depth, at or below the tile's surface, and the negated offsets
 * otherwise. An absent unit is skipped. */
void battle_gfx_apply_misc_unit_palette_modulation(
    s32 preset_color, s32 time, s32 misc_id, s16 red, s16 green, s16 blue) {
    SVECTOR screen_coordinates;
    battle_unit_misc_data_t* unit;
    map_tile_t* tile;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);
    if (unit == 0) {
        return;
    }

    battle_unit_copy_misc_data_xyz_values(&screen_coordinates, unit);
    tile = battle_map_get_tile_data_pointer(
        screen_coordinates.vx / MAP_TILE_SIZE, screen_coordinates.vz / MAP_TILE_SIZE, unit->map_z);

    battle_map_modify_palette(preset_color, time, MAP_CLUT_ROW_UNITS, misc_id, 0, red, green, blue);

    if ((tile->depth_half_height.value & MAP_TILE_DEPTH_MASK) != 0
        && unit->screen.vy >= -(tile->height + (tile->depth_half_height.value >> MAP_TILE_DEPTH_SHIFT)) * 12) {
        battle_map_modify_palette(preset_color, time, MAP_CLUT_ROW_UNIT_SHADOWS, misc_id, 0, red, green, blue);
    } else {
        battle_map_modify_palette(preset_color, time, MAP_CLUT_ROW_UNIT_SHADOWS, misc_id, 0, -red, -green, -blue);
    }
}
