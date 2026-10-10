#include "fft/event_jobstts.h"
#include "psx/libgpu.h"

/* Stream command: set the sprite CLUT to GetClut(data[3] * 16,
 * data[4] | data[2] << 8) and return the next command. */
const u8* jobstts_cmd_set_clut_handler(const u8* data) {
    g_jobstts_gfx_clut_id = GetClut(data[3] << 4, data[4] | (data[2] << 8));
    return data + data[1];
}
