#include "fft/battle_gfx.h"
#include "psx/types.h"

/* battle_gfx_calculate_screen_z_from_input_coords at the current x and map-depth
 * position of the unit, or of its mount for a rider, on the mount's map layer. A
 * rider's result is moved by -10 plus the mount's float bob offset. */
s32 battle_gfx_calculate_screen_z_from_misc_screen_data(battle_unit_t* unit) {
    SVECTOR coord;
    battle_unit_t* mount;
    s32 adjusted_screen_z;
    s32 screen_z;

    mount = battle_unit_get_mount_misc_data(unit);
    /* The target loads the s16 screen coordinates unsigned (lhu). */
    coord.vx = mount->screen.vx;
    coord.vz = mount->screen.vz;
    coord.vy = mount->map_z;
    screen_z = battle_gfx_calculate_screen_z_from_input_coords(mount, &coord);
    if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_RIDER) {
        adjusted_screen_z = screen_z - 10;
        screen_z = mount->float_bob_offset + adjusted_screen_z;
    }
    return (s16)screen_z;
}
