#include "fft/wldcore.h"
#include "psx/types.h"

/* Location script opcode 35 (EraseLocation): end the script with a location's dot to hide. It does
 * not step past its operand; the script ends there anyway. */
void wldcore_script_request_erase_location(void) {
    g_wldcore_script_flags |= WLDCORE_SCRIPT_REQUEST_ERASE_LOCATION;
    g_wldcore_script_state.args[0] = g_wldcore_script_state.data[g_wldcore_script_state.ip];
}
