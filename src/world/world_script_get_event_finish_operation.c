#include "fft/world.h"
#include "psx/types.h"

/* WORLD's copy of battle_script_get_event_finish_operation: the after-event code of the event being
 * finished (g_world_next_event_id), bits 8-9 and 12-15 of its 16-bit finish-operation value
 * shifted down by 8. */
u32 world_script_get_event_finish_operation(void) {
    return (g_main_scenario_event_finish_operations[g_world_next_event_id] & 0xF300) >> 8;
}
