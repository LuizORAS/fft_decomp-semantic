#include "fft/battle_camera.h"
#include "fft/battle_unit.h"
#include "fft/script.h"
#include "psx/types.h"

/* Battle twin of world_unit_check_facing_relative_to_camera: nonzero when the
 * unit's facing, relative to the camera yaw quadrant, is one of the six
 * "away from camera" directions 7..12. */
s32 battle_unit_check_facing_relative_to_camera(s32 unit_id) {
    s32 runtime_id;
    s32 camera_facing;

    runtime_id = script_resolve_runtime_id(unit_id);
    camera_facing = camera_wrap_yaw_angle();
    camera_facing += 0x200;
    camera_facing &= 0xf00;
    /* Preserve the retail arithmetic shift after the masked camera angle. */
    camera_facing >>= 8;
    camera_facing += battle_unit_get_facing_nibble_by_runtime_id(runtime_id);
    camera_facing &= 0xf;
    camera_facing -= 7;
    return camera_facing < 6U;
}
