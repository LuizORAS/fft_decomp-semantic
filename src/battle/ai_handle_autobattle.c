#include "fft/battle_ai.h"

/*
 * Dispatch the acting unit's special automatic-battle behavior.
 *
 * A result of -2 chains recovery into retreat, or fading-life behavior into
 * recovery. All dispatched routines otherwise collapse success to 1 while
 * preserving the suspended result -1.
 */
s32 ai_handle_autobattle(void) {
    s32 result;

    /* A -2 result falls back to another policy's case; those jumps stay gotos. */
    switch (g_ai_data_base.autobattle_setting) {
    case AI_AUTOBATTLE_TARGETED:
        if (ai_select_targeted_action() != -1) {
            return 1;
        }
        break;
    case AI_AUTOBATTLE_RECOVERY:
    recovery_policy:
        result = ai_select_recovery_action();
        if (result == -1) {
            return -1;
        }
        if (result == -2) {
            goto retreat_policy;
        }
        return 1;
    case AI_AUTOBATTLE_FADING_LIFE:
        result = ai_select_fading_life_action();
        if (result == -1) {
            return -1;
        }
        if (result == -2) {
            goto recovery_policy;
        }
        return 1;
    case AI_AUTOBATTLE_RETREAT:
    retreat_policy:
        if (ai_select_retreat_action() != -1) {
            return 1;
        }
        break;
    default:
        return 0;
    }
    return -1;
}
