#include "fft/wldcore.h"
#include "psx/types.h"

/* Run the formation screen an event asked for before it starts
 * (EVENT_SCRIPT_VAR_FORMATION_RETURN_EVENT): clear MAIN_SYSTEM_FLAG_RESTART, run
 * world_formation_run_before_event and return 1, so wldcore_entrypoint returns 0 and the event
 * plays. Returns 0 when no event is waiting. */
s32 wldcore_system_run_requested_formation(void) {
    if (world_script_get_variable(EVENT_SCRIPT_VAR_FORMATION_RETURN_EVENT) != 0) {
        g_main_system_flags &= ~MAIN_SYSTEM_FLAG_RESTART;
        world_formation_run_before_event();
        return 1;
    }

    return 0;
}
