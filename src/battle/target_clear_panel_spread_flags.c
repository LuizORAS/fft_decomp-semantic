#include "fft/battle_action.h"
#include "psx/types.h"

/* Clear the mark byte (the spread frontier) of all 512 targeting panels. */
void target_clear_panel_spread_flags(void) {
    s32 i;
    target_panel_t* panel;

    i = 0x1FF;
    panel = &g_target_panels[511];
    do {
        panel->mark = 0;
        i -= 1;
        panel--;
    } while (i >= 0);
}
