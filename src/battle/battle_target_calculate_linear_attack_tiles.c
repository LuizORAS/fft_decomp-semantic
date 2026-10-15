#include "fft/battle.h"
#include "psx/types.h"

/* Mark the in-range tiles, on both layers, on the line from (x, y), that tile included, in
 * direction dir (BATTLE_TARGET_LINE_EAST +x, _WEST -x, _NORTH +y, _SOUTH -y; any other value
 * marks nothing), up to 32 tiles or the map edge. */
void battle_target_calculate_linear_attack_tiles(s32 dir, s32 x, s32 y) {
    s32 i;
    s32 dx;
    s32 dy;
    s32 map_width;
    battle_target_panel_t* panel;

    switch (dir) {
    case BATTLE_TARGET_LINE_EAST:
        dx = 1;
        dy = 0;
        break;
    case BATTLE_TARGET_LINE_WEST:
        dx = -1;
        dy = 0;
        break;
    case BATTLE_TARGET_LINE_NORTH:
        dx = 0;
        dy = 1;
        break;
    case BATTLE_TARGET_LINE_SOUTH:
        dx = 0;
        dy = -1;
        break;
    default:
        return;
    }

    i = 0;
    do {
        if (x < 0) {
            return;
        }
        map_width = g_battle_map_max_x;
        if (x >= map_width) {
            return;
        }
        if (y < 0) {
            return;
        }
        if (y >= g_battle_map_max_y) {
            return;
        }
        dir = y * map_width + x;
        panel = &g_battle_target_panels[dir];
        if (panel->remaining_range != 0) {
            panel->mark = 1;
        }
        dir += 0x100;
        panel = &g_battle_target_panels[dir];
        y += dy;
        if (panel->remaining_range != 0) {
            panel->mark = 1;
        }
        i += 1;
        x += dx;
    } while (i < 0x20);
}
