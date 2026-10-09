#include "fft/world.h"
#include "psx/types.h"

s32 world_input_read_tutorial_or_controller(s32 unused_0) {
    u32 input;

    if (g_world_script_tutorial_id != 0) {
        input = g_world_script_tutorial_shift_command_bit;
    } else {
        input = PadRead(0);
    }
    if (input == MAIN_INPUT_RESET_BUTTONS) {
        world_noop_801325d4();
        world_game_reset();
    }
    return input;
}
