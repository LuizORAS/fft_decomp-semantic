#include "fft/event_bunit.h"
#include "psx/types.h"

/* Set the sprite CLUT from the command's normal or compact-layout coordinates
 * and return the next command. */
menu_instruction_t* bunit_cmd_set_layout_clut_handler(menu_instruction_t* command) {
    if (g_bunit_menu_use_alternate_palette != 0) {
        g_bunit_gfx_clut_id = GetClut(command->clut.alternate_x_div_16 << 4,
            command->clut.alternate_y_low | (command->clut.alternate_y_high << 8));
    } else {
        g_bunit_gfx_clut_id = GetClut(
            command->clut.normal_x_div_16 << 4, command->clut.normal_y_low | (command->clut.normal_y_high << 8));
    }
    return menu_next_instruction(command);
}
