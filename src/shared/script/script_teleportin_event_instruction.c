#include "fft/battle_effect.h"
#include "fft/script.h"
#include "fft/thread.h"

/* Start the teleport-in effect for an event unit. The interpreter supplies 1
 * as its second argument; this entry forwards only the unit ID. */
void script_teleportin_event_instruction(s32 unit_id, s32 unused) {
    s32 misc_id = script_get_misc_id(unit_id);
    if (misc_id != EVENT_MISC_ID_NONE) {
        g_thread_call_target = (void (*)(void))effect_set_secondary_teleport_by_misc_id;
        thread_call_on_main_stack(misc_id);
    }
}
