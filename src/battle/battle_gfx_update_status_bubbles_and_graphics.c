#include "fft/battle.h"
#include "psx/types.h"

/* Per-frame status bubbles and unit graphics. In the menu, move and targeting states the status
 * bubbles are drawn (battle_gfx_update_status_bubble_display on every misc record) once the camera has
 * been still for 0x3c frames; a zoom, tilt, rotation or camera move restarts that delay. In every
 * state the casting unit's graphics are loaded and each unit's sprite layers drawn
 * (battle_gfx_draw_unit_sprite_layers). */
void battle_gfx_update_status_bubbles_and_graphics(void) {
    battle_unit_misc_data_t* unit;

    switch (g_battle_game_state) {
    case BATTLE_GAME_STATE_FREE_CURSOR:
    case BATTLE_GAME_STATE_HIGHLIGHT_UNITS:
    case BATTLE_GAME_STATE_OPEN_ACTION_MENUS:
    case BATTLE_GAME_STATE_IDLING_ACTION_MENUS:
    case BATTLE_GAME_STATE_AI_COMMAND:
    case BATTLE_GAME_STATE_DISPLAY_MOVE_AREA:
    case BATTLE_GAME_STATE_UNIT_MOVE:
    case BATTLE_GAME_STATE_SELECT_MOVE_TILE:
    case BATTLE_GAME_STATE_TARGETING_MESSAGE:
    case BATTLE_GAME_STATE_TARGETING_RANGE:
    case BATTLE_GAME_STATE_ABILITY_PREVIEW_HANDLING:
    case BATTLE_GAME_STATE_CONFIRM_ACTION:
        if (g_battle_camera_zoom_action == 0 && g_battle_camera_tilt_action == 0 && g_battle_camera_rotation_action == 0
            && !(g_battle_current_vector.vx | g_battle_current_vector.vy | g_battle_current_vector.vz)) {
            if (g_battle_gfx_status_bubble_delay != 0) {
                g_battle_gfx_status_bubble_delay -= g_battle_state_vsync_interval;
                if (g_battle_gfx_status_bubble_delay <= 0) {
                    g_battle_gfx_status_bubble_delay = 0;
                }
            } else {
                for (unit = g_battle_unit_last_misc_data; unit != 0; unit = unit->previous) {
                    battle_gfx_update_status_bubble_display(unit);
                }
            }
        } else {
            g_battle_gfx_status_bubble_delay = 0x3c;
        }
        break;
    }
    battle_gfx_load_casting_unit_graphics();
    for (unit = g_battle_unit_last_misc_data; unit != 0; unit = unit->previous) {
        battle_gfx_draw_unit_sprite_layers(unit);
    }
}
