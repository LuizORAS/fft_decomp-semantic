#include "fft/battle_gfx.h"

/* battle_gfx_calculate_screen_z_from_input_coords at the caller's coords (vy
 * holds the map layer), evaluated for the mount when the unit is a rider. A
 * rider's result is moved by -10 plus the mount's float bob offset. */
s32 battle_gfx_calculate_screen_z_with_caller_data(battle_unit_t* unit, const SVECTOR* coords) {
    battle_unit_t* mount;
    s32 adjusted_screen_z;
    s32 screen_z;

    mount = battle_unit_get_mount_misc_data(unit);
    screen_z = battle_gfx_calculate_screen_z_from_input_coords(mount, coords);
    if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_RIDER) {
        adjusted_screen_z = screen_z - 10;
        screen_z = mount->float_bob_offset + adjusted_screen_z;
    }
    return (s16)screen_z;
}
