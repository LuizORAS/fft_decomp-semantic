#include "fft/event_jobstts.h"
#include "fft/menu.h"

/* Opcode 0x0f in JOBSTTS's display scripts: byte 3 becomes the draw priority (g_menu_gfx_otag_index)
 * unless a temporary draw area is active (g_jobstts_gfx_otag_index_locked, set by
 * jobstts_menu_script_draw_opening_window_frame while a window opens). Returns the next command. */
menu_instruction_t* jobstts_menu_script_set_draw_priority(menu_instruction_t* data) {
    if (g_jobstts_gfx_otag_index_locked == 0) {
        g_menu_gfx_otag_index = data->draw_state.value;
    }
    return menu_next_instruction(data);
}
