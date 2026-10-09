#include "fft/battle.h"
#include "psx/types.h"

s32 battle_state_update_deployment_controller_input(void) {
    u32 previous;
    u32 current;

    current = PadRead(1);
    previous = g_main_input_previous_buttons;
    g_main_input_current_buttons = current;
    g_main_input_previous_buttons = current;
    g_main_input_pressed_buttons = ~previous & current;
    g_main_input_released_buttons = ~current & previous;

    if ((current & MAIN_INPUT_RESET_BUTTONS) == MAIN_INPUT_RESET_BUTTONS) {
        main_system_reset_game();
    }
    return 1;
}
