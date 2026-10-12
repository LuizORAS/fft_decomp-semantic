#include "fft/battle_target.h"
#include "psx/types.h"

/* Clear the range and the mark of all 512 targeting panels. */
void target_clear_panel_data(void) {
    s32 i = 0;
    target_panel_t* panel = g_target_panel_data;
    for (; i < 512; i++) {
        panel->remaining_range = 0;
        panel->mark = 0;
        panel++;
    }
}
