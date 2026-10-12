#include "fft/battle_action.h"
#include "psx/types.h"

/* Clear the range and the mark of all 512 targeting panels. */
void battle_target_clear_panel_data(void) {
    s32 i = 0;
    battle_target_panel_t* panel = g_battle_target_panel_data;
    for (; i < 512; i++) {
        panel->remaining_range = 0;
        panel->mark = 0;
        panel++;
    }
}
