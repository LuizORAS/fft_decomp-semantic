#include "fft/world.h"
#include "psx/types.h"

/* Bits 8..9 and 12..15 of the 16-bit finish-operation value of the event being
 * finished, shifted down to bit 0. */
u32 world_script_get_scenario_finish_operation_flags(void) {
    return (g_main_scenario_event_finish_operations[g_world_finish_operation_event_id] & 0xF300) >> 8;
}
