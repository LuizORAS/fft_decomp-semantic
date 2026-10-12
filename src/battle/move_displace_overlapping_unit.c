#include "fft/battle_move.h"
#include "psx/types.h"

/* Pushes a unit sharing the mover's tile aside as the mover steps off-centre.
 *
 * The push is 3/2 of the mover's distance short of 7 pixels past the tile
 * centre along direction. It is skipped while the mover's step before last
 * carries movement flag 0x10. The case order and per-case centre temporaries
 * reproduce the target's cross-jumped tails. */
void move_displace_overlapping_unit(battle_unit_misc_data_t* unit, s32 direction) {
    battle_unit_misc_data_t* other;
    s32 dist;
    s32 x;
    s32 y;
    s32 center;

    other = battle_unit_get_overlapping_misc_data_pointer(unit, unit->map_x, unit->map_y, unit->map_z);
    if (unit->movement_path_offset < 2) {
        return;
    }
    if (unit->path.data.steps[unit->movement_path_offset - 2].mount) {
        return;
    }
    if (other == 0) {
        return;
    }
    x = other->map_x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    y = other->map_y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    switch (direction) {
    case MOVE_DIRECTION_NORTH:
        center = unit->map_y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
        dist = unit->screen.vz - center;
        break;
    case MOVE_DIRECTION_SOUTH:
        center = unit->map_y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
        dist = center - unit->screen.vz;
        break;
    case MOVE_DIRECTION_WEST:
        center = unit->map_x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
        dist = center - unit->screen.vx;
        break;
    case MOVE_DIRECTION_EAST:
        center = unit->map_x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
        dist = unit->screen.vx - center;
        break;
    }
    if (dist >= 8) {
        dist = 7;
    }
    dist -= 7;
    move_displace_unit_along_step_direction(other, x, y, -dist * 3 / 2);
}
