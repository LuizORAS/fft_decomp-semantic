#include "fft/battle.h"

/* TeleportIn (EVENT_OPCODE_TELEPORT_IN): play the teleport secondary effect on the unit, on the main
 * stack. A unit id that does not resolve does nothing.
 *
 * The interpreter passes 1 as a second argument, as it does for TeleportOut's removal flag; this entry
 * does not read it. */
void battle_script_teleport_in_event_instruction(s32 unit_id, s32 unused) {
    s32 misc_id = battle_script_get_misc_id(unit_id);
    if (misc_id != EVENT_MISC_ID_NONE) {
        g_battle_thread_call_target = (void (*)(void))battle_effect_set_secondary_teleport_by_misc_id;
        battle_thread_call_on_main_stack(misc_id);
    }
}
