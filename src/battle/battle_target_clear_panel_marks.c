#include "fft/battle_target.h"
#include "psx/types.h"

/* Clear the mark of all 512 tile panels, from the last one down. */
void battle_target_clear_panel_marks(void) {
    s32 i;
    battle_target_panel_t* panel;

    for (i = 511, panel = &g_battle_target_panels[511]; i >= 0; i--, panel--) {
        panel->mark = 0;
    }
}
