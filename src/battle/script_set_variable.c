#include "fft/battle_script.h"
#include "fft/script.h"

/* Store a value in a script variable, whichever of the 32-bit variable, flag and nibble
 * ranges it belongs to, by running the ZeroVariable and AddImmediate commands on
 * it. 32-bit script variable 0 (the comparison result) is preserved around them, and war
 * funds are capped at 99,999,999. */
void script_set_variable(s32 variable_id, s32 value) {
    s32 saved_current_variable = *g_script_variables;

    if (variable_id == EVENT_SCRIPT_VAR_WAR_FUNDS && value > 0x05f5e0ff) {
        value = 0x05f5e0ff;
    }
    script_run_variable_command(EVENT_OPCODE_ZERO_VARIABLE, variable_id, 0, 0);
    script_run_variable_command(EVENT_OPCODE_ADD_IMMEDIATE, variable_id, value, 0);
    *g_script_variables = saved_current_variable;
}
