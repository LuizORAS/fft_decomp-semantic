#include "fft/event_equip.h"
#include "psx/types.h"

/* Run the menu command stream `menu` with `state`, forcing state 0 whenever `override` is set. */
void equip_menu_dispatch_with_override(u8* menu, s32 state, s32 override) {
    s32 effective_state;

    effective_state = state;
    if (override != 0) {
        effective_state = 0;
    }
    equip_menu_set_palette_mode(override);
    equip_menu_run_display_script(menu, effective_state);
}
