#include "fft/battle_gfx.h"
#include "fft/battle_unit.h"

/* Snaps a unit to its map tile: the movement destination becomes the tile and the
 * screen position its centre (28 units per tile) at the tile's height. */
void battle_unit_set_move_and_screen_coords(battle_unit_t* unit) {
    unit->movement.destination_x = unit->map_x;
    unit->movement.destination_y = unit->map_y;
    unit->movement.destination_z = unit->map_z;
    unit->screen.vx = unit->map_x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    unit->screen.vz = unit->map_y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    unit->screen.vy = battle_gfx_calculate_screen_z_from_misc_screen_data(unit);
    battle_unit_set_real_coords_from_screen_coords(unit);
}
