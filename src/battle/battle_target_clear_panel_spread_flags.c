#include "fft/battle_action.h"
#include "psx/types.h"

/* Clear the mark byte (the spread frontier) of all 512 targeting panels. */
void battle_target_clear_panel_spread_flags(void) {
    s32 i;
    battle_target_panel_t* panel;

    i = 0x1FF;
    panel = &g_battle_target_panels[511];
    do {
        panel->mark = 0;
        i -= 1;
        panel--;
    } while (i >= 0);
}
