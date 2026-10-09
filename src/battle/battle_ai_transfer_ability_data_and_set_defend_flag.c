#include "fft/battle.h"
#include "psx/types.h"

/* Reset the selected action to the candidate coordinates for the current
 * movement scenario; the Defend skillset marks a Defend when the unit has it. */
void battle_ai_transfer_ability_data_and_set_defend_flag(void) {
    u8 scenario = g_battle_ai_data_base.movement_scenario;
    battle_stats_t* unit = g_battle_ai_data_base.acting_unit;
    u32 coords;
    s32 support;

    g_battle_ai_data_base.selected_action.act_before_move = 0;
    coords = g_battle_ai_data_base.candidate_coords[scenario].word;
    support = unit->support_ability;
    g_battle_ai_data_base.selected_action.coords.word = coords;
    if (support == ABILITY_ID_SUPPORT_DEFEND) {
        g_battle_ai_data_base.selected_action.skillset = SKILLSET_ID_DEFEND;
    } else {
        g_battle_ai_data_base.selected_action.skillset = 0;
    }
}
