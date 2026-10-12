#include "fft/battle_ai.h"

/* With an ally in peril, switch to the recovery policy and return -2.
 * Otherwise focus on the unit's auto-battle target and run the general search;
 * -1 suspends. */
s32 ai_select_action_fading_life(void) {
    if (g_ai_data_base.resume_pending == 0) {
        if (ai_select_peril_target() != 0) {
            g_ai_data_base.autobattle_setting = AI_AUTOBATTLE_RECOVERY;
            return -2;
        }
        ai_focus_on_target(g_ai_data_base.acting_unit->auto_battle_target);
    }
    if (ai_select_action_general() == -1)
        return -1;
    return 0;
}
