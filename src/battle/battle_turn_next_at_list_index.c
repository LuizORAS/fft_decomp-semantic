#include "fft/battle_action.h"
#include "psx/types.h"

/* Advance the AT list browse, wrapping after 40 entries. */
void battle_turn_next_at_list_index(void) {
    g_battle_turn_at_list_index++;
    if (g_battle_turn_at_list_index >= BATTLE_AT_LIST_CAPACITY) {
        g_battle_turn_at_list_index = 0;
    }
}
