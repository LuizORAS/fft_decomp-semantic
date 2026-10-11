#include "fft/battle.h"

/* With an ally in peril, switch to the recovery policy and return -2.
 * Otherwise focus on the unit's auto-battle target and run the general search;
 * -1 suspends. */
s32 battle_ai_select_fading_life_action(void) {
    if (g_battle_ai_data_base.resume_pending == 0) {
        if (battle_ai_select_peril_target() != 0) {
            g_battle_ai_data_base.autobattle_setting = BATTLE_AI_AUTOBATTLE_RECOVERY;
            return -2;
        }
        battle_ai_store_main_target_id_and_focus_on_target_flag(g_battle_ai_data_base.acting_unit->auto_battle_target);
    }
    if (battle_ai_run_non_specific_autobattle() == -1)
        return -1;
    return 0;
}
