#include "fft/battle.h"
#include "psx/types.h"

/* Resets the camera for a new map: screen offset (0x100, 0xa0, 0x280), zoom ONE,
 * pitch 0x12e (the low tilt), yaw 0xe00, position at the origin, cursor on tile
 * (0, 0), no acting or casting unit, no rotation, tilt or zoom in progress and
 * directional panning off. */
void battle_camera_init_defaults(void) {
    main_util_set_vector(&g_battle_offset_screen_coords, 0x100, 0xA0, 0x280);
    main_util_set_vector(&g_battle_camera_zoom, ONE, ONE, ONE);
    main_util_set_svector(&g_battle_camera_angles, 0x12E, 0xE00, 0);
    main_util_set_vector(&g_battle_camera_current_real_coords, 0, 0, 0);

    g_battle_casting_unit_id = 0xFF;
    g_battle_unit_source_misc_id = 0xFF;
    g_battle_cursor_y = 0;
    g_battle_cursor_z = 0;
    g_battle_cursor_x = 0;
    g_battle_camera_rotation_action = BATTLE_CAMERA_ROTATION_NONE;
    g_battle_camera_zoom_action = BATTLE_CAMERA_STEP_NONE;
    g_battle_camera_zoom_target = BATTLE_CAMERA_ZOOM_IN;
    g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_NONE;
    g_battle_camera_tilt_target = BATTLE_CAMERA_TILT_LOW;
    g_battle_state_camera_pan_enabled = 0;
}
