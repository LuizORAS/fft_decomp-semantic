#include "fft/battle.h"
#include "psx/types.h"

/* Deliberately argument-free: the target sets up no arguments before this
 * call. The real `(s32 unit_id, s32 removal_only)` prototype in fft/battle.h is
 * reached through a function-pointer cast at the call, so the header can be
 * included here. */

/* Run battle_status_resolve_unit_changes as a preview (action state PREVIEW during the call, EXECUTE
 * after) with no arguments, so unit_id and removal_only are whatever $a0 and $a1 hold (QUIRKS.md).
 * Nothing on the disc references it.
 *
 * The real (s32 unit_id, s32 removal_only) prototype in fft/battle.h is reached through a
 * function-pointer cast at the call, so the header can be included here. */
void battle_status_resolve_unit_changes_in_preview(void) {
    g_battle_action_state = BATTLE_ACTION_STATE_PREVIEW;
    ((void (*)(void))battle_status_resolve_unit_changes)();
    g_battle_action_state = BATTLE_ACTION_STATE_EXECUTE;
}
