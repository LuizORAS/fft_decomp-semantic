#include "fft/battle.h"
#include "psx/types.h"

enum {
    RESET_BUTTON_MASK = 0x090c,
};

/* Same pad words for the setup screens, with the soft-reset combination. Returns 1. */
s32 battle_state_update_deployment_controller_input(void) {
    u32 previous;
    u32 current;

    current = PadRead(1);
    previous = g_controller_input_previous;
    g_controller_input_raw = current;
    g_controller_input_previous = current;
    g_controller_input_pressed = ~previous & current;
    g_controller_input_released = ~current & previous;

    if ((current & RESET_BUTTON_MASK) == RESET_BUTTON_MASK) {
        main_system_reset_game();
    }
    return 1;
}
