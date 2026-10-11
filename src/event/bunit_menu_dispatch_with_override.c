#include "fft/event_bunit.h"
#include "psx/types.h"

/* BUNIT.OUT 001c85f8 - Run the menu command stream `menu` with `state`, but clear `state` to
 * 0 whenever the override flag is set (also passed to bunit_menu_set_palette_mode). */
void bunit_menu_dispatch_with_override(u8* menu, s32 state, s32 override) {
    s32 effective_state = state;
    if (override != 0) {
        effective_state = 0;
    }
    bunit_menu_set_palette_mode(override);
    bunit_cmd_run_stream(menu, effective_state);
}
