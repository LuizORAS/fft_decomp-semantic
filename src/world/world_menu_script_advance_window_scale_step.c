#include "fft/world.h"
#include "psx/types.h"

/* Opcode 0x16: advance the window opening animation one step (capped at 4) and move past the
 * one-byte command. */
menu_instruction_t* world_menu_script_advance_window_scale_step(menu_instruction_t* instruction) {
    s8 step = g_world_menu_window_scale_step;

    if (step < 4) {
        g_world_menu_window_scale_step = step + 1;
    }
    return (menu_instruction_t*)((u8*)instruction + 1);
}
