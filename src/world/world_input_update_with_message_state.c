#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Once a frame on the formation, name entry and shop screens: read the controller (with
 * the tutorial's scripted input), then drop the input while the window scales (steps 1-3),
 * while thread 1 runs or a grid menu is open, during a fade, and on the frame after thread 1's
 * task ends. While thread 1 runs with g_menu_input_lock_mode set, g_world_text_task_phase is
 * 1 on the first frame (queuing the window-open sound) and 2 after; once thread 1 stops, the
 * task flag and the held sound loop are cleared. world_card_input_update_with_message_state, a
 * second copy of this text for the memory card screen, reads world_input_update_controller and
 * keeps its own phase byte, g_world_input_blocked_state. */
void world_input_update_with_message_state(void) {
    s32 running = 0;
    s32 step;
    u8 value;

    world_tutorial_update_controller_input();
    step = world_menu_get_window_scale_step();
    if (step >= 1 && step <= 3) {
        world_input_clear_state();
    } else {
        running = thread_is_running(THREAD_SCREEN_SLOT_TEXT);
        if (running != 0 || g_world_grid_menu_id != MENU_GRID_HELP_NONE) {
            world_input_clear_state();
            if (running != 0 && g_menu_input_lock_mode != 0) {
                value = 1;
                if (g_world_text_task_phase != 0) {
                    value = 2;
                }
                g_world_text_task_phase = value;
            } else {
                g_world_text_task_phase = 0;
            }
        } else if (g_world_text_task_phase != 0 || world_gfx_get_fade_state() != WORLD_FADE_STATE_NONE) {
            g_world_text_task_phase = 0;
            world_input_clear_state();
        }
    }
    if (running == 0) {
        g_menu_input_lock_mode = 0;
        g_world_sound_release_held_loop = 0;
    }
    if (g_world_text_task_phase == 1) {
        g_world_menu_sound_effect_id = MAIN_SFX_WINDOW_OPEN;
    }
}
