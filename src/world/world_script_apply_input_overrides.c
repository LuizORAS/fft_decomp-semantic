#include "fft/world.h"
#include "psx/types.h"

/* Refresh the low halfword of a menu state word from the disabled and forced
 * controller-input variables, preserving its high halfword. The reset
 * button combination requests a full game reset before the refresh. */
void world_script_apply_input_overrides(u32* state) {
    u32 high;

    if (*state == MAIN_INPUT_RESET_BUTTONS) {
        main_system_reset_game();
    }

    high = *state & 0xFFFF0000;
    *state = *state & (world_script_get_variable(EVENT_SCRIPT_VAR_DISABLED_CONTROLLER_INPUTS) ^ 0xFFFF);
    *state = high | (world_script_get_variable(EVENT_SCRIPT_VAR_FORCED_CONTROLLER_INPUTS) | *state);
}
