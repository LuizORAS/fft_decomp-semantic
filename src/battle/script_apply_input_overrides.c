#include "fft/battle_script.h"
#include "fft/main.h"
#include "fft/script.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Apply the script's input overrides to a 32-bit button value in place: clear the buttons
 * named by the disabled-inputs variable and set those named by the forced-inputs
 * variable, keeping the 32-bit value's high 16 bits. Start + Select + L1 + R1 held alone
 * resets the game first. */
void script_apply_input_overrides(u32* input) {
    u32 high_buttons;

    if (*input == (PSX_PAD_START | PSX_PAD_SELECT | PSX_PAD_L1 | PSX_PAD_R1)) {
        main_system_reset_game();
    }

    high_buttons = *input & 0xffff0000;
    *input &= script_get_variable(EVENT_SCRIPT_VAR_DISABLED_CONTROLLER_INPUTS) ^ 0xffff;
    *input = high_buttons | (script_get_variable(EVENT_SCRIPT_VAR_FORCED_CONTROLLER_INPUTS) | *input);
}
