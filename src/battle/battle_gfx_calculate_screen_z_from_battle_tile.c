#include "fft/battle_gfx.h"
#include "psx/types.h"

/*
 * Calculate screen Z at the grounded unit's map-tile center.
 *
 * A mounted rider uses the mount's map position, then applies its mounted
 * height offset and the original -10 adjustment.
 */
s32 battle_gfx_calculate_screen_z_from_battle_tile(battle_unit_t* unit) {
    SVECTOR coord;
    battle_stats_t* battle_data;
    battle_unit_t* mount;
    s32 adjusted_screen_z;
    s32 screen_z;

    mount = battle_unit_get_mount(unit);
    battle_data = mount->battle_data;
    coord.vx = battle_data->x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    battle_data = mount->battle_data;
    coord.vz = battle_data->position.y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    coord.vy = mount->map_z;
    screen_z = battle_gfx_calculate_screen_z_from_input_coords(mount, &coord);
    if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_RIDER) {
        adjusted_screen_z = screen_z - 10;
        screen_z = mount->float_bob_offset + adjusted_screen_z;
    }
    return (s16)screen_z;
}
