#include "fft/battle_action.h"
#include "psx/types.h"

/* Advance the AT list cursor, wrapping after the list's 40 entries. */
void action_increment_at_list_id(void) {
    g_action_at_list_id++;
    if (g_action_at_list_id >= BATTLE_AT_LIST_CAPACITY) {
        g_action_at_list_id = 0;
    }
}
