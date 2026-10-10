#include "fft/battle.h"

/* Step the pitch toward the tilt target, 0x1c0 (high) or 0x12e (low), from
 * either side, by an increment that grows by twice the animation speed each
 * frame until it passes 0x2f. The tilt ends on the target. */
void battle_camera_update_tilt(void) {
    s16 tilt;

    if (g_battle_camera_tilt_action == BATTLE_CAMERA_STEP_INCREASE
        && g_battle_map_tilt_target == g_battle_camera_tilt_action) {
        s16* state = &g_battle_camera_render_state.vx;
        tilt = *state + g_battle_camera_tilt_increment;
        *state = tilt;
        if (tilt >= 0x1C0) {
            g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_NONE;
            *state = 0x1C0;
        }
    } else if (g_battle_camera_tilt_action == BATTLE_CAMERA_STEP_DECREASE
        && g_battle_map_tilt_target == BATTLE_MAP_TILT_LOW) {
        s16* state = &g_battle_camera_render_state.vx;
        tilt = *state - g_battle_camera_tilt_increment;
        *state = tilt;
        if (tilt < 0x12F) {
            g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_NONE;
            *state = 0x12E;
        }
    } else if (g_battle_camera_tilt_action == BATTLE_CAMERA_STEP_DECREASE
        && g_battle_map_tilt_target == BATTLE_MAP_TILT_HIGH) {
        s16* state = &g_battle_camera_render_state.vx;
        tilt = *state - g_battle_camera_tilt_increment;
        *state = tilt;
        if (tilt < 0x1C1) {
            g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_NONE;
            *state = 0x1C0;
        }
    } else if (g_battle_camera_tilt_action == BATTLE_CAMERA_STEP_INCREASE
        && g_battle_map_tilt_target == BATTLE_MAP_TILT_LOW) {
        s16* state = &g_battle_camera_render_state.vx;
        tilt = *state + g_battle_camera_tilt_increment;
        *state = tilt;
        if (tilt >= 0x12E) {
            g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_NONE;
            *state = 0x12E;
        }
    } else {
        return;
    }
    if (g_battle_camera_tilt_increment < 0x30) {
        g_battle_camera_tilt_increment += g_main_system_animation_speed * 2;
    }
}
