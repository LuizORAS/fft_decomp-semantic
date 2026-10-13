#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* The memory card screen's variant of world_input_update_with_message_state: it reads the controller
 * directly, not through the tutorial input, and keeps its phase in g_world_input_blocked_state. */
void world_card_input_update_with_message_state(void) {
    s32 running = 0;
    s32 step;
    u8 value;

    world_input_update_controller();
    step = world_menu_get_window_scale_step();
    if (step >= 1 && step <= 3) {
        world_input_clear_state();
    } else {
        running = thread_is_running(THREAD_SCREEN_SLOT_TEXT);
        if (running != 0 || g_world_grid_menu_id != MENU_GRID_HELP_NONE) {
            world_input_clear_state();
            if (running != 0 && g_world_thread_task_active != 0) {
                value = 1;
                if (g_world_input_blocked_state != 0) {
                    value = 2;
                }
                g_world_input_blocked_state = value;
            } else {
                g_world_input_blocked_state = 0;
            }
        } else if (g_world_input_blocked_state != 0 || world_gfx_get_fade_state() != WORLD_FADE_STATE_NONE) {
            g_world_input_blocked_state = 0;
            world_input_clear_state();
        }
    }
    if (running == 0) {
        g_world_thread_task_active = 0;
        g_world_sound_release_held_loop = 0;
    }
    if (g_world_input_blocked_state == 1) {
        g_world_menu_sound_effect_id = MAIN_SFX_WINDOW_OPEN;
    }
}
