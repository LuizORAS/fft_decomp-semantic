#include "fft/event_equip.h"
#include "fft/menu_script.h"

/* Run a display script with `state`, forcing state 0 (no input) while the palette is dimmed
 * (`override` set); EQUIP compiles this text too (equip_menu_dispatch_with_override). Type debt
 * (QUIRKS.md): script is a display script (byte stream), but the callers declare their scripts as u8[],
 * s16[] or menu_instruction_t. */
void menu_run_script_with_palette_mode(menu_instruction_t* script, s32 input, s32 mode) {
    s32 value;

    value = input;
    if (mode != 0) {
        value = 0;
    }
    equip_menu_set_palette_mode(mode);
    equip_menu_run_display_script(script, value);
}
