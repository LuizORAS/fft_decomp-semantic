#include "fft/event_bunit.h"
#include "fft/menu.h"
#include "fft/menu_script.h"

/* Opcode 0x15 (MENU_SCRIPT_OP_REPEAT_DRAW_AREA): unless a temporary draw area is active
 * (g_bunit_gfx_otag_index_locked), queue the screen's draw area (g_bunit_gfx_draw_area, its y reset to 0)
 * at the draw priority + 1. Returns the next command. */
menu_instruction_t* menu_script_reregister_temporary_draw_area(menu_instruction_t* data) {
    if (g_bunit_gfx_otag_index_locked == 0) {
        s32 otag_index = g_menu_gfx_otag_index;
        g_bunit_gfx_draw_area.y = 0;
        menu_gfx_enqueue_draw_area(&g_bunit_gfx_draw_area, otag_index + 1);
    }
    return menu_next_instruction(data);
}
