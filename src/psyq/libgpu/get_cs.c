/* LIBGPU 80025824-800258f0. */
#include "psx/libgpu.h"

u32 get_cs(int x, int y) {
    int value;
    int y0;
    register u32 ybits __asm__("$3");
    register u32 xbits __asm__("$2");
    u8* type;
    register int max __asm__("$6"); /* The first clamp limit remains in a2. */
    u16* limit;

    x = (s16)x;
    if (x < 0) {
        value = 0;
    } else {
        limit = &g_psyq_gpu_vram_width;
        __asm__("" : "=r"(limit) : "0"(limit)); /* The original materializes the limit address. */
        value = *limit;
        __asm__("" : "=r"(value) : "0"(value)); /* Keep the original unsigned load followed by signed narrowing. */
        value = (s16)value;
        max = value - 1;
        if (max < x) {
            x = max;
        }
        value = x;
    }
    x = value;
    value = (u32)y << 16;
    y0 = value >> 16;
    y = 0;
    if (y0 >= 0) {
        limit = &g_psyq_gpu_vram_height;
        __asm__("" : "=r"(limit) : "0"(limit)); /* The original materializes the limit address. */
        value = *limit;
        __asm__("" : "=r"(value) : "0"(value)); /* Keep the original unsigned load followed by signed narrowing. */
        y = (s16)value - 1;
        if (y < y0) {
            y0 = y;
        }
        y = y0;
    }
    type = &g_psyq_gpu_graph_type;
    __asm__("" : "=r"(type) : "0"(type)); /* The original materializes the environment address. */
    if ((u32)(*type - 1) >= 2) {
        ybits = (y & 0x3ff) << 10;
        xbits = x & 0x3ff;
    } else {
        ybits = (y & 0xfff) << 12;
        xbits = x & 0xfff;
    }
    xbits |= PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_AREA_TOP_LEFT);
    return ybits | xbits;
}
