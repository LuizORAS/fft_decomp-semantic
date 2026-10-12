#include "fft/battle_ai.h"
#include "psx/types.h"

/* Reset the selected action to the candidate coordinates for the current
 * movement scenario; the Defend skillset marks a Defend when the unit has it. */
void ai_select_action_fallback(void) {
    u8 scenario = g_ai_data_base.movement_scenario;
    battle_stats_t* unit = g_ai_data_base.acting_unit;
    u32 coords;
    s32 support;

    g_ai_data_base.selected_action.act_before_move = 0;
    coords = g_ai_data_base.candidate_coords[scenario].word;
    support = unit->abilities.support;
    g_ai_data_base.selected_action.coords.word = coords;
    if (support == ABILITY_ID_SUPPORT_DEFEND) {
        g_ai_data_base.selected_action.command.skillset_id = SKILLSET_ID_DEFEND;
    } else {
        g_ai_data_base.selected_action.command.skillset_id = 0;
    }
}
