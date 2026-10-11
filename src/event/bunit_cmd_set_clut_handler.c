#include "fft/event_bunit.h"
#include "psx/libgpu.h"

/* Select the sprite CLUT from the encoded VRAM coordinates. */
menu_instruction_t* bunit_cmd_set_clut_handler(menu_instruction_t* data) {
    g_bunit_gfx_clut_id
        = GetClut(data->fixed_clut.x_div_16 << 4, data->fixed_clut.y_low | (data->fixed_clut.y_high << 8));
    return menu_next_instruction(data);
}
