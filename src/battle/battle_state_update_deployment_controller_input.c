#include "fft/battle.h"
#include "psx/types.h"

enum {
    RESET_BUTTON_MASK = 0x090c,
};

s32 battle_state_update_deployment_controller_input(void) {
    u32 previous;
    u32 current;

    current = PadRead(1);
    previous = g_main_input_previous_buttons;
    g_main_input_current_buttons = current;
    g_main_input_previous_buttons = current;
    g_main_input_pressed_buttons = ~previous & current;
    g_main_input_released_buttons = ~current & previous;

    if ((current & RESET_BUTTON_MASK) == RESET_BUTTON_MASK) {
        main_system_reset_game();
    }
    return 1;
}
