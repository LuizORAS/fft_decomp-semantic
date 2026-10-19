#include "fft/menu_script.h"

/* Set the sprite CLUT from the command's first coordinate triple (bytes 2-4)
 * in palette mode 0, or its second (bytes 5-7) in the alternate palette
 * mode, and return the next command. */
menu_instruction_t* menu_script_set_clut(menu_instruction_t* command) {
    if (g_menu_use_alternate_palette != 0) {
        g_menu_gfx_clut_id = GetClut(command->clut.alternate_x_div_16 << 4,
            command->clut.alternate_y_low | (command->clut.alternate_y_high << 8));
    } else {
        g_menu_gfx_clut_id = GetClut(
            command->clut.normal_x_div_16 << 4, command->clut.normal_y_low | (command->clut.normal_y_high << 8));
    }
    return menu_next_instruction(command);
}
