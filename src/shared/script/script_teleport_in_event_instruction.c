#include "fft/battle_effect.h"
#include "fft/script.h"
#include "fft/thread.h"

/* TeleportIn (EVENT_OPCODE_TELEPORT_IN): play the teleport secondary effect on the unit, on the main
 * stack. A unit id that does not resolve does nothing. BATTLE and WORLD compile this text.
 *
 * The interpreter passes 1 as a second argument, as it does for TeleportOut's removal flag; this entry
 * does not read it. */
void script_teleport_in_event_instruction(s32 unit_id, s32 unused) {
    s32 runtime_id = script_resolve_runtime_id(unit_id);
    if (runtime_id != EVENT_RUNTIME_ID_NONE) {
        g_thread_call_target = (void (*)(void))battle_effect_set_secondary_teleport_by_runtime_id;
        thread_call_on_main_stack(runtime_id);
    }
}
