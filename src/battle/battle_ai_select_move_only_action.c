#include "fft/battle.h"
#include "psx/types.h"

/* g_battle_ai_temp_unit_data names g_battle_ai_data_base.acting_unit separately:
 * a member access makes GCC share one base register where the target keeps a
 * lui per site. */

/* Make the selected action only moving to the current scenario's destination, Defending there when
 * the unit's support ability is Defend: the fallback when no ranked action is taken. */
void battle_ai_select_move_only_action(void) {
    u8 scenario = g_battle_ai_data_base.movement_scenario;
    battle_stats_t* unit = g_battle_ai_temp_unit_data;
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
