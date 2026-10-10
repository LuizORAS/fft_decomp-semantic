#include "fft/battle.h"

/* Choose the turn's action from the ranked list: walk the eight entries best first and take each
 * with its hit chance (rand() % 100 below base_hit_percent), so a sure action is always taken when
 * reached and a 30% one only 30% of the time; if every roll fails, take the first entry. Returns 0,
 * choosing nothing, when every entry has a 0% chance, else 1. */
s32 battle_ai_select_ranked_action(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 i;
    s32 any;
    s32 chance;

    any = 0;
    for (i = 0; i < 8; i++) {
        chance = ai->ranked_actions[i].base_hit_percent;
        if (chance != 0) {
            any = 1;
        }
        if ((rand() % 100) < chance) {
            battle_ai_transfer_halfword_values(
                (u16*)&ai->selected_action, (u16*)&ai->ranked_actions[i], sizeof(battle_ai_action_data_t));
            return 1;
        }
    }
    if (any == 0) {
        return 0;
    }
    battle_ai_transfer_halfword_values(
        (u16*)&ai->selected_action, (u16*)&ai->ranked_actions[0], sizeof(battle_ai_action_data_t));
    return 1;
}
