#include "fft/event_jobstts.h"
#include "fft/menu.h"
#include "psx/types.h"

/* Run a display script (menu_script_opcode_e) until JOBSTTS_MENU_SCRIPT_END (0x16): clear the draw
 * priority, its temporary-draw-area lock and semi-transparency, keep `flags` as the input the handlers read
 * (g_jobstts_menu_display_script_input), dispatch each command through g_jobstts_menu_script_handlers, then
 * advance the transition frame (g_jobstts_gfx_transition_frame) by one, up to 10: JOBSTTS's opening clips
 * count frames by script runs. */
void jobstts_menu_run_display_script(menu_instruction_t* instruction, s32 flags) {
    g_jobstts_gfx_otag_index_locked = 0;
    g_menu_gfx_otag_index = 0;
    g_jobstts_gfx_semitransparency = 0;
    g_jobstts_menu_display_script_input = flags;
    while (instruction->header.opcode != JOBSTTS_MENU_SCRIPT_END) {
        instruction = g_jobstts_menu_script_handlers[instruction->header.opcode](instruction);
    }
    if (g_jobstts_gfx_transition_frame < 10) {
        g_jobstts_gfx_transition_frame = g_jobstts_gfx_transition_frame + 1;
    }
}
