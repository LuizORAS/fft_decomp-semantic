#include "fft/world.h"

/* Write event script variable `variable_id` the way a script does, through
 * the Zero and Add-immediate variable commands; the comparison-result word
 * they use is saved and restored. The war funds are capped at 99,999,999. */
void world_script_set_variable(s32 variable_id, s32 value) {
    s32 saved_current_variable = *g_world_script_variables;

    if (variable_id == EVENT_SCRIPT_VAR_WAR_FUNDS && value > 0x05f5e0ff) {
        value = 0x05f5e0ff;
    }
    world_script_run_variable_command(EVENT_OPCODE_ZERO_VARIABLE, variable_id, 0, 0);
    world_script_run_variable_command(EVENT_OPCODE_ADD_IMMEDIATE, variable_id, value, 0);
    *g_world_script_variables = saved_current_variable;
}
