#include "fft/battle.h"
#include "psx/pad.h"
#include "psx/types.h"

void battle_camera_update_rotation(void) {
    s32 angle;

    if (g_battle_camera_rotation_action == BATTLE_CAMERA_ROTATION_LEFT) {
        u16* yaw = (u16*)&g_battle_camera_render_state.vy;
        *yaw -= g_battle_camera_rotation_increment;
        if ((s16)*yaw <= g_battle_destination_camera_yaw) {
            angle = (u16)g_battle_destination_camera_yaw & 0xFFF;
            *yaw = angle;
            if (g_main_input_previous_buttons & PSX_PAD_L1) {
                if (g_battle_game_state != BATTLE_GAME_STATE_EVENT) {
                    battle_camera_rotate_left((angle & 0xFE0) - 0x400);
                }
            } else {
                g_battle_camera_rotation_action = BATTLE_CAMERA_ROTATION_NONE;
                main_sound_stop_sfx(MAIN_SFX_CAMERA_ROTATE_RIGHT);
                main_sound_stop_sfx(MAIN_SFX_CAMERA_ROTATE_LEFT);
                main_sound_play_sfx_find_channel(MAIN_SFX_CAMERA_ROTATE_STOP);
            }
        }
    } else if (g_battle_camera_rotation_action == BATTLE_CAMERA_ROTATION_RIGHT) {
        u16* yaw = (u16*)&g_battle_camera_render_state.vy;
        *yaw += g_battle_camera_rotation_increment;
        if ((s16)*yaw >= g_battle_destination_camera_yaw) {
            angle = (u16)g_battle_destination_camera_yaw & 0xFFF;
            *yaw = angle;
            if (g_main_input_previous_buttons & PSX_PAD_R1) {
                if (g_battle_game_state != BATTLE_GAME_STATE_EVENT) {
                    battle_camera_rotate_left((angle & 0xFE0) + 0x400);
                }
            } else {
                g_battle_camera_rotation_action = BATTLE_CAMERA_ROTATION_NONE;
                main_sound_stop_sfx(MAIN_SFX_CAMERA_ROTATE_RIGHT);
                main_sound_stop_sfx(MAIN_SFX_CAMERA_ROTATE_LEFT);
                main_sound_play_sfx_find_channel(MAIN_SFX_CAMERA_ROTATE_STOP);
            }
        }
    } else {
        return;
    }
    if (g_battle_camera_rotation_increment < 0x30) {
        g_battle_camera_rotation_increment = (g_main_system_animation_speed * 2) + g_battle_camera_rotation_increment;
    }
}
