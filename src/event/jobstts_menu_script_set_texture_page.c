#include "fft/event_jobstts.h"

/* Stream command: set the texture page from the command (colour mode
 * data[4], blend rate data[2] >> 4, x data[3] * 16, y data[2] * 256) and
 * return the next command. */
const u8* jobstts_menu_script_set_texture_page(const u8* data) {
    g_jobstts_gfx_texture_page = GetTPage(data[4], data[2] >> 4, data[3] << 4, data[2] << 8);
    return data + data[1];
}
