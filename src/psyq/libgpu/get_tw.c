/* LIBGPU 80025a04-80025a88. */
#include "psx/libgpu.h"

u32 get_tw(RECT* rect) {
    u32 pad[4];
    if (rect) {
        pad[0] = (rect->x & 0xff) >> 3;
        pad[2] = (-rect->w & 0xff) >> 3;
        pad[1] = (rect->y & 0xff) >> 3;
        pad[3] = (-rect->h & 0xff) >> 3;
        return PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_TEXTURE_WINDOW) | (pad[1] << 15) | (pad[0] << 10) | (pad[3] << 5)
            | pad[2];
    }
    return 0;
}
