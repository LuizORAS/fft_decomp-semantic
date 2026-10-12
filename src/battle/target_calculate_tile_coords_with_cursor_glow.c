#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/*
 * Tile coordinate calculation with the cursor glow, used while an AT list is
 * active (see target_calculate_tile_coords_and_glow_from_at_list).
 */
void target_calculate_tile_coords_with_cursor_glow(void) {
    SVECTOR secondary;
    SVECTOR tertiary;
    SVECTOR corner0;
    SVECTOR corner1;
    SVECTOR corner2;
    SVECTOR corner3;
    VECTOR primary;
    s32 depth;
    s32 result;
    map_tile_t* tile;

    target_project_cursor_tile_to_screen(&primary, &secondary, &tertiary);
    /* The projected z is the ordering-table depth for the cursor glow. */
    depth = primary.vz / 4;
    battle_gfx_draw_map_selection_cursor(&secondary, main_gfx_get_otag());
    /* The polygon routine reads the same three 16-bit values in renderer x, z, y order. */
    result = target_calculate_cursor_tile_polygon(&secondary, g_battle_cursor_z, &corner0);
    tile = battle_map_get_tile_data_pointer(
        secondary.vx / MAP_TILE_SIZE, secondary.vz / MAP_TILE_SIZE, g_battle_cursor_z);
    battle_gfx_build_cursor_tile_glow(
        tile->flags.untargetable, result, &corner0, &corner1, &corner2, &corner3, main_gfx_get_otag() + depth);
}
