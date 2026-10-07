#include "fft/battle.h"
#include "psx/types.h"

/* Restart the AT list browse at its first entry. */
void battle_turn_clear_at_list_index(void) {
    g_battle_turn_at_list_index = 0;
}
