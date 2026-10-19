#include "fft/event_jobstts.h"
#include "fft/menu.h"

/* Opcode 0x12 in JOBSTTS's display scripts, its form of MENU_SCRIPT_OP_OPENING_WINDOW_FRAME: draw the
 * window frame (jobstts_menu_script_draw_window_frame) through a clip that opens vertically. For the first
 * 8 transition frames the clip spans the 256-pixel screen width and the window's height scaled by
 * g_jobstts_gfx_transition_height_percent about its centre (below frame 3 the script input is cleared),
 * and the frame draws one priority above it. WORLD's and the other overlays' form scale both sides for 4
 * steps. */
menu_instruction_t* jobstts_menu_script_draw_opening_window_frame(menu_instruction_t* command) {
    menu_instruction_t* next;
    RECT rect;

    if (g_jobstts_gfx_transition_frame < 8) {
        s32 scale;

        if (g_jobstts_gfx_transition_frame < 3) {
            g_jobstts_menu_display_script_input = 0;
        }
        scale = g_jobstts_gfx_transition_height_percent[g_jobstts_gfx_transition_frame];
        rect.w = 0x100;
        rect.h = (command->window.height * scale) / 100;
        rect.x = 0;
        rect.y = (command->window.y + (command->window.height >> 1)) - (rect.h >> 1);
        jobstts_gfx_enqueue_draw_area(&rect, g_menu_gfx_otag_index);
        g_jobstts_gfx_otag_index_locked = 1;
        g_menu_gfx_otag_index = g_menu_gfx_otag_index + 1;
    } else {
        g_jobstts_gfx_otag_index_locked = 0;
    }
    next = jobstts_menu_script_draw_window_frame(command);
    g_menu_gfx_otag_index++;
    return next;
}
