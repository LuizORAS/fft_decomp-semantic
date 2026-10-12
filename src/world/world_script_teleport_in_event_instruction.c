#include "fft/battle.h"
#include "fft/world.h"

/* WORLD's copy of battle_script_teleport_in_event_instruction (TeleportIn): play the teleport effect
 * on the unit.
 *
 * The interpreter also passes a second argument, 1; this entry does not read it. */
void world_script_teleport_in_event_instruction(s32 unit_id, s32 unused) {
    s32 misc_id;

    misc_id = world_script_get_misc_id(unit_id);
    if (misc_id != EVENT_MISC_ID_NONE) {
        g_world_thread_call_target = (void (*)(void))battle_effect_set_secondary_teleport_by_misc_id;
        world_thread_call_on_main_stack(misc_id);
    }
}
