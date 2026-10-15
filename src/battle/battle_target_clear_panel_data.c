#include "fft/battle.h"
#include "psx/types.h"

/* Clear the range and mark of all 512 tile panels. */
void battle_target_clear_panel_data(void) {
    s32 i = 0;
    battle_target_panel_t* panel = g_battle_target_panels;
    for (; i < 512; i++) {
        panel->remaining_range = 0;
        panel->mark = 0;
        panel++;
    }
}
