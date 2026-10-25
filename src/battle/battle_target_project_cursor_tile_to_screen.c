#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_map.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Project the cursor tile to the screen.
 *
 * The tile center (x * 28 + 14, surface height, y * 28 + 14) is raised by the
 * tile's depth, offset by the modify-by-5 or -by-1 conversion depending on
 * surface bit 0x40, and transformed by the battle camera. The projected X and
 * Y are kept in g_battle_cursor_screen_x/g_battle_cursor_screen_y for battle_camera_update_cursor_tile_vector. */
void battle_target_project_cursor_tile_to_screen(VECTOR* projected, SVECTOR* position, SVECTOR* raised) {
    map_tile_t* tile;
    long flag;

    battle_gfx_update_unit_palette_animation();
    SetRotMatrix(&g_battle_camera_matrix);
    SetTransMatrix(&g_battle_camera_matrix);
    main_util_set_svector(position, g_battle_cursor_x * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2), 0,
        g_battle_cursor_y * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2));
    position->vy = battle_map_calculate_slope_height(position, (u8)g_battle_cursor_z);
    tile = battle_map_get_tile_data_pointer(
        (position->vx / BATTLE_MAP_TILE_SIZE), (position->vz / BATTLE_MAP_TILE_SIZE), g_battle_cursor_z);
    position->vy -= tile->depth_half_height.depth * 12;
    if (tile->surface.depth_bias) {
        battle_camera_get_tile_sort_point_biased(position, raised);
    } else {
        battle_camera_get_tile_sort_point_unbiased(position, raised);
    }
    RotTrans(raised, projected, &flag);
    g_battle_cursor_screen_x = projected->vx;
    g_battle_cursor_screen_y = projected->vy;
}
