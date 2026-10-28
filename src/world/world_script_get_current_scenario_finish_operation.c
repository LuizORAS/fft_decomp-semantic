#include "fft/script.h"
#include "fft/world.h"

/* Return the active scenario's event-finish operations, or 0 outside a scenario. */
s32 world_script_get_current_scenario_finish_operation(void) {
    u16 result;

    if (g_world_scenario_event_active != 0) {
        result = g_main_scenario_event_finish_operations[script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT)];
    } else {
        result = 0;
    }
    return result;
}
