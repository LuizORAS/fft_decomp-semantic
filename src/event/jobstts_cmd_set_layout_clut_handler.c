#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Select the sprite CLUT for the current palette mode. */
menu_instruction_t* jobstts_cmd_set_layout_clut_handler(menu_instruction_t* cmd) {
    if (g_jobstts_menu_use_alternate_palette != 0) {
        g_jobstts_gfx_clut_id
            = GetClut(cmd->clut.alternate_x_div_16 << 4, cmd->clut.alternate_y_low | (cmd->clut.alternate_y_high << 8));
    } else {
        g_jobstts_gfx_clut_id
            = GetClut(cmd->clut.normal_x_div_16 << 4, cmd->clut.normal_y_low | (cmd->clut.normal_y_high << 8));
    }
    return menu_next_instruction(cmd);
}
