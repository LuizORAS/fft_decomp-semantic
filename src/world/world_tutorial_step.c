#include "fft/world.h"

/* Per-frame step of the tutorial being played; world_formation_run_screen calls it every frame.
 * Returns 1 while no tutorial runs or the current one goes on, and the END command's result when it
 * ends.
 *
 * Reads pad 0 as the tutorial's own button input (the soft-reset combination resets the game), draws
 * the highlight ring while its brightness is nonzero (world_tutorial_advance_highlight), and runs the
 * command at g_world_tutorial_command_ptr: a byte below WORLD_TUTORIAL_CMD_PRESS_BUTTON_LIMIT presses
 * a button, the others show or change a message, draw or hide the highlight ring, wait, or end. */
s32 world_tutorial_step(void) {
    s32 pad;
    u8 cmd;

    if (g_world_tutorial_id == 0) {
        return 1;
    }
    g_world_tutorial_button_input = PadRead(0);
    if (g_world_tutorial_button_input == MAIN_INPUT_RESET_BUTTONS) {
        world_input_reset_game();
    }
    g_world_tutorial_buttons = g_world_tutorial_button_input;
    if (g_world_tutorial_highlight_brightness != 0) {
        world_tutorial_advance_highlight();
    }
    cmd = *g_world_tutorial_command_ptr;
    if (cmd < WORLD_TUTORIAL_CMD_PRESS_BUTTON_LIMIT) {
        world_tutorial_command_press_button();
    } else if (cmd == WORLD_TUTORIAL_CMD_DISPLAY_MESSAGE) {
        world_tutorial_command_display_message();
    } else if (cmd == WORLD_TUTORIAL_CMD_HIGHLIGHT_RING) {
        world_tutorial_command_highlight_ring();
    } else if (cmd == WORLD_TUTORIAL_CMD_HIDE_HIGHLIGHT) {
        world_tutorial_command_hide_highlight();
    } else if (cmd == WORLD_TUTORIAL_CMD_WAIT_TIME) {
        world_tutorial_command_wait_time();
    } else if (cmd == WORLD_TUTORIAL_CMD_WAIT_FOR_BUTTON) {
        world_tutorial_command_wait_for_button();
    } else if (cmd == WORLD_TUTORIAL_CMD_END) {
        return world_tutorial_command_end();
    } else if (cmd == WORLD_TUTORIAL_CMD_WAIT_FOR_MESSAGE) {
        world_tutorial_command_wait_for_message();
    } else if (cmd == WORLD_TUTORIAL_CMD_CHANGE_DIALOG) {
        world_tutorial_command_change_dialog();
    }
    return 1;
}
