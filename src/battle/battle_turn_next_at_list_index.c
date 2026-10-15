#include "fft/battle.h"
#include "psx/types.h"

/* Advance the AT list browse, wrapping after 40 entries. */
void battle_turn_next_at_list_index(void) {
    s32 id = g_battle_turn_at_list_index + 1;

    g_battle_turn_at_list_index = id;
    if (id >= 0x28) {
        g_battle_turn_at_list_index = 0;
    }
}
