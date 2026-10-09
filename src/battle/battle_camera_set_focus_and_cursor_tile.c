#include "fft/battle.h"
#include "psx/types.h"

void battle_camera_set_focus_and_cursor_tile(const SVECTOR* coords, const SVECTOR* rotation) {
    s32 tile_x;
    s32 tile_y;

    main_util_set_vector(&g_battle_camera_current_real_coords, -coords->vx << 12, -coords->vy << 12, -coords->vz << 12);
    main_util_set_svector(
        &g_battle_camera_render_state, rotation->vx & 0x1fff, rotation->vy & 0x1fff, rotation->vz & 0x1fff);
    g_battle_cursor_z = 0;
    tile_x = g_battle_camera_current_real_coords.vx / 0x1c000;
    g_battle_cursor_x = tile_x;
    tile_y = g_battle_camera_current_real_coords.vz / 0x1c000;
    g_battle_cursor_y = tile_y;
    g_battle_camera_current_real_coords.vy = -battle_map_get_tile_data_pointer(tile_x, tile_y, 0)->height * 0xc000;
}
