#include "fft/main.h"
#include "fft/script.h"

/* Apply the script's input overrides to a 32-bit button state in place: clear the buttons
 * named by the disabled-inputs variable and set those named by the forced-inputs
 * variable, keeping its high 16 bits. Start + Select + L1 + R1 held alone
 * resets the game first. */
void script_apply_input_overrides(u32* state) {
    u32 high;

    if (*state == MAIN_INPUT_RESET_BUTTONS) {
        main_system_reset_game();
    }

    high = *state & 0xFFFF0000;
    *state = *state & (script_get_variable(EVENT_SCRIPT_VAR_DISABLED_CONTROLLER_INPUTS) ^ 0xFFFF);
    *state = high | (script_get_variable(EVENT_SCRIPT_VAR_FORCED_CONTROLLER_INPUTS) | *state);
}
