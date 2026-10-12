#include "fft/battle_camera.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Step the three zoom scales toward the zoom target, ONE zooming in or 0xc00
 * zooming out, by an increment that grows by four times the animation speed
 * each frame until it reaches 0x300. The zoom ends on the target. */
void battle_camera_update_zoom(void) {
    s32 increment;
    s32* zoom_ptr;

    if (g_battle_camera_zoom_action == BATTLE_CAMERA_STEP_INCREASE && g_battle_map_zoom_target == BATTLE_MAP_ZOOM_IN) {
        s32 zoom;
        s32 below;
        s32 pan_x;
        s32 pan_y;

        zoom_ptr = &g_battle_camera_zoom.vx;
        zoom = *zoom_ptr;
        increment = g_battle_camera_zoom_increment;
        zoom = zoom + increment;
        *zoom_ptr = zoom;
        below = zoom < ONE;
        pan_x = g_battle_camera_zoom.vy;
        pan_y = g_battle_camera_zoom.vz;
        pan_x = pan_x + increment;
        pan_y = pan_y + increment;
        g_battle_camera_zoom.vy = pan_x;
        g_battle_camera_zoom.vz = pan_y;
        if (!below) {
            g_battle_camera_zoom_action = BATTLE_CAMERA_STEP_NONE;
            g_battle_camera_zoom.vz = ONE;
            g_battle_camera_zoom.vy = ONE;
            *zoom_ptr = ONE;
        }
    } else if (g_battle_camera_zoom_action == BATTLE_CAMERA_STEP_DECREASE
        && g_battle_map_zoom_target == g_battle_camera_zoom_action) {
        s32 zoom;
        s32 below;

        zoom_ptr = &g_battle_camera_zoom.vx;
        zoom = *zoom_ptr;
        increment = g_battle_camera_zoom_increment;
        zoom = zoom - increment;
        *zoom_ptr = zoom;
        below = zoom < 0xC01;
        g_battle_camera_zoom.vy -= increment;
        g_battle_camera_zoom.vz -= increment;
        if (below) {
            g_battle_camera_zoom_action = BATTLE_CAMERA_STEP_NONE;
            g_battle_camera_zoom.vz = 0xC00;
            g_battle_camera_zoom.vy = 0xC00;
            *zoom_ptr = 0xC00;
        }
    } else {
        return;
    }
    if (increment < 0x300) {
        g_battle_camera_zoom_increment = g_main_system_animation_speed * 4 + increment;
    }
}
