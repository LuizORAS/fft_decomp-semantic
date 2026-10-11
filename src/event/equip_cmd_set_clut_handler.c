#include "fft/event_equip.h"
#include "psx/libgpu.h"

/* Stream command: set the sprite CLUT to GetClut(data->fixed_clut.x_div_16 * 16,
 * data->fixed_clut.y_low | data->fixed_clut.y_high << 8) and return the next command. */
menu_instruction_t* equip_cmd_set_clut_handler(menu_instruction_t* data) {
    g_equip_gfx_clut_id
        = GetClut(data->fixed_clut.x_div_16 << 4, data->fixed_clut.y_low | (data->fixed_clut.y_high << 8));
    return menu_next_instruction(data);
}
