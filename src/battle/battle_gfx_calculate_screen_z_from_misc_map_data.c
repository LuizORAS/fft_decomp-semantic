#include "fft/battle.h"
#include "psx/types.h"

/* battle_gfx_calculate_screen_z_from_input_coords at the centre of the map tile
 * of the unit, or of its mount for a rider. A rider's result is moved by -10 plus
 * the mount's float bob offset. */
s32 battle_gfx_calculate_screen_z_from_misc_map_data(battle_unit_misc_data_t* unit) {
    SVECTOR coord;
    battle_unit_misc_data_t* mount;
    s32 adjusted_screen_z;
    s32 screen_z;

    mount = battle_unit_get_mount_misc_data(unit);
    coord.vx = mount->map_x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    coord.vz = mount->map_y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    coord.vy = mount->map_z;
    screen_z = battle_gfx_calculate_screen_z_from_input_coords(mount, &coord);
    if (unit->mount_state == BATTLE_MISC_MOUNT_STATE_RIDER) {
        adjusted_screen_z = screen_z - 10;
        screen_z = mount->float_bob_offset + adjusted_screen_z;
    }
    return (s16)screen_z;
}
