#include "fft/battle_action.h"
#include "psx/types.h"

/* Advance the AT list cursor, wrapping after the list's 40 entries. */
void action_increment_at_list_id(void) {
    s32 id = g_action_at_list_id + 1;

    g_action_at_list_id = id;
    if (id >= BATTLE_AT_LIST_CAPACITY) {
        g_action_at_list_id = 0;
    }
}
