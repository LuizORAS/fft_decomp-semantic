#include "fft/event_jobstts.h"
#include "psx/libgpu.h"

/* Select the sprite CLUT from the command's VRAM coordinates. */
menu_instruction_t* jobstts_cmd_set_clut_handler(menu_instruction_t* data) {
    g_jobstts_gfx_clut_id
        = GetClut(data->fixed_clut.x_div_16 << 4, data->fixed_clut.y_low | (data->fixed_clut.y_high << 8));
    return menu_next_instruction(data);
}
