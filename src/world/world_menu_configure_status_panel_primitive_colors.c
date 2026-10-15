#include "fft/world.h"
#include "psx/types.h"

/*
 * Select the status-panel sprite palettes and tint its five trailing sprites.
 *
 * Keep the pointer-walk expression: subscript arithmetic changes induction
 * variables and does not reproduce the target loop registers.
 */
void world_menu_configure_status_panel_primitive_colors(
    battle_menu_status_panel_buffer_t* primitives, const world_status_thread_t* state) {
    s32 i;
    s32 clut;

    if (state->style == 1 || g_world_menu_input_lock_mode == 1) {
        clut = 0x7dfc;
        for (i = 11; i >= 0; i--) {
            (primitives->sprites + i)->clut = clut;
        }
        for (i = 12; i < 19; i++) {
            (primitives->sprites + i)->clut = 0x7d3c;
        }
        primitives->sprites[0].clut = 0x7c7c;
        primitives->sprites[1].clut = 0x7c7c;
        for (i = 0; i < 5; i++) {
            SetShadeTex(&primitives->sprites[i + 19], 0);
            (primitives->sprites + i + 19)->r0 = 0x40;
            (primitives->sprites + i + 19)->g0 = 0x40;
            (primitives->sprites + i + 19)->b0 = 0x80;
        }
    } else {
        clut = 0x7d7c;
        for (i = 11; i >= 0; i--) {
            (primitives->sprites + i)->clut = clut;
        }
        for (i = 12; i < 19; i++) {
            (primitives->sprites + i)->clut = 0x7c3c;
        }
        primitives->sprites[0].clut = 0x7cbc;
        primitives->sprites[1].clut = 0x7cbc;
        for (i = 0; i < 5; i++) {
            SetShadeTex(&primitives->sprites[i + 19], 0);
            (primitives->sprites + i + 19)->r0 = 0x80;
            (primitives->sprites + i + 19)->g0 = 0x80;
            (primitives->sprites + i + 19)->b0 = 0x80;
        }
    }
}
