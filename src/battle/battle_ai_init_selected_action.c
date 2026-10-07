#include "fft/battle.h"
#include "psx/types.h"

/* Reset the selected action's ranking (battle_ai_reset_action_ranking_fields) so the next
 * battle_ai_build_command plans again: at a unit's new turn, after a crystal or chest pickup, after a
 * sprung trap and after a failed teleport. */
void battle_ai_init_selected_action(void) {
    battle_ai_reset_action_ranking_fields(&g_battle_ai_data_base.selected_action);
}
