#include "fft/script.h"
#include "fft/wldcore.h"

/* Location script opcode 28 (SetVar): set the script variable to the operand at once, whenever the
 * script runs, and go on. */
void wldcore_script_set_var(void) {
    s32 variable_id;
    s32 value;

    wldcore_script_read_operand_pair(&variable_id, &value);
    script_set_variable(variable_id, value);
}
