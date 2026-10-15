#include "fft/wldcore.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Build the map camera matrix (the rotation, the origin as its translation, the scale) and project
 * the 13 x 17 tile corners, storing each screen position. A corner outside x -160..128 or
 * y -152..120 is marked WLDCORE_MAP_TILE_OFF_SCREEN; empty tiles keep their flags. */
void wldcore_map_project_and_cull_tiles(void) {
    MATRIX matrix;
    SVECTOR position;
    VECTOR projected;
    long flag;
    s32 i;
    wldcore_projected_entry_t* entry;
    u8* rotation;
    s32 flags;
    s32 updated;
    s32 coordinate;

    rotation = g_wldcore_map_projection_rotation_bytes;
    RotMatrix(rotation, &matrix);
    TransMatrix(&matrix, rotation - 0x20);
    ScaleMatrix(&matrix, rotation - 0x10);
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);

    i = 0;
    entry = g_wldcore_map_projected_tiles;
    for (; i < 221; i++) {
        coordinate = g_wldcore_map_projected_tiles[i].map_x;
        position.vx = coordinate;
        coordinate = g_wldcore_map_projected_tiles[i].map_y;
        position.vz = 0;
        position.vy = coordinate;
        RotTrans(&position, &projected, &flag);
        g_wldcore_map_projected_tiles[i].screen_x = projected.vx;
        g_wldcore_map_projected_tiles[i].screen_y = projected.vy;
        flags = entry->flags;
        if (!(flags & WLDCORE_MAP_TILE_EMPTY)) {
            if (projected.vx < -0xa0 || projected.vx > 0x80 || projected.vy < -0x98 || projected.vy > 0x78) {
                updated = flags | WLDCORE_MAP_TILE_OFF_SCREEN;
            } else {
                updated = flags & ~WLDCORE_MAP_TILE_OFF_SCREEN;
            }
            entry->flags = updated;
        }
        entry++;
    }
}
