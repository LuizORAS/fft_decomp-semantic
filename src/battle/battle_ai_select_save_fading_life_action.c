#include "fft/battle_ai.h"

/* Save fading life: when an ally is in peril (battle_ai_select_peril_target makes it the Auto-Battle
 * target), switch the mode to Protect allies and return -2 so battle_ai_select_auto_battle_action runs it.
 * Otherwise focus on the target battle_ai_select_peril_target left (the most hurt non-supporter) and
 * run the general search. Returns -1 to suspend, else 0. */
s32 battle_ai_select_save_fading_life_action(void) {
    if (g_battle_ai_data_base.resume_pending == 0) {
        if (battle_ai_select_peril_target() != 0) {
            g_battle_ai_data_base.autobattle_setting = BATTLE_AUTO_BATTLE_PROTECT_ALLIES;
            return -2;
        }
        battle_ai_focus_on_target(g_battle_ai_data_base.acting_unit->auto_battle_target);
    }
    if (battle_ai_select_general_action() == -1)
        return -1;
    return 0;
}
