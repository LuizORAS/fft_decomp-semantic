#include "fft/world.h"
#include "psx/types.h"

/* Tutorial command below WORLD_TUTORIAL_CMD_PRESS_BUTTON_LIMIT: press pad bit number <command> for
 * one frame (g_world_tutorial_pressed_button, which world_input_read_tutorial_or_controller hands the
 * screen as its pad), then wait 30 frames (counted by the VSync mode) before moving to the next
 * command. */
void world_tutorial_command_press_button(void) {
    if (g_world_tutorial_command_active != 0) {
        g_world_tutorial_command_active -= world_gfx_get_vsync_mode_or_one();
        g_world_tutorial_pressed_button = 0;
        if (g_world_tutorial_command_active <= 0) {
            g_world_tutorial_command_active = 0;
            ++g_world_tutorial_command_ptr;
        }
    } else {
        g_world_tutorial_command_active = 0x1E;
        g_world_tutorial_pressed_button = 1 << *g_world_tutorial_command_ptr;
    }
}
