#include "fft/script.h"
#include "fft/wldcore.h"
#include "psx/types.h"

/* Location script opcode 18 (Casualties>=): fail the script when the casualty count is below the
 * operand. */
void wldcore_script_check_casualties_at_least(void) {
    u16* ip = &g_wldcore_script_state.ip;
    u16 idx = *ip;
    u16 value = g_wldcore_script_state.data[idx];
    *ip = idx + 1;
    if (script_get_variable(EVENT_SCRIPT_VAR_CASUALTIES) < value) {
        g_wldcore_script_state.flags |= WLDCORE_SCRIPT_RESULT_CONDITION_FAILED;
    }
}
