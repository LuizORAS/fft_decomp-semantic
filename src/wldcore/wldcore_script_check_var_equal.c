#include "fft/script.h"
#include "fft/wldcore.h"
#include "psx/types.h"

/* Location script opcode 1 (Var=): fail the script when the script variable differs from the
 * operand. */
void wldcore_script_check_var_equal(void) {
    s32 variable_id;
    s32 value;

    wldcore_script_read_operand_pair(&variable_id, &value);
    if (script_get_variable(variable_id) != value) {
        g_wldcore_script_state.flags |= WLDCORE_SCRIPT_RESULT_CONDITION_FAILED;
    }
}
