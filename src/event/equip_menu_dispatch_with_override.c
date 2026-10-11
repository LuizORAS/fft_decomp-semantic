#include "fft/event_equip.h"
#include "psx/types.h"

/* Run the script command stream `script` with `state`, forcing state 0 whenever `override` is set. */
void equip_menu_dispatch_with_override(menu_instruction_t* script, s32 state, s32 override) {
    s32 effective_state;

    effective_state = state;
    if (override != 0) {
        effective_state = 0;
    }
    equip_menu_set_palette_mode(override);
    equip_cmd_run_stream(script, effective_state);
}
