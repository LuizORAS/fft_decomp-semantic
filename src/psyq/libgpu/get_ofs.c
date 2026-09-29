/* LIBGPU 800259bc-80025a04. */
#include "psx/libgpu.h"

u32 get_ofs(int x, int y) {
    u8* type = &g_psyq_gpu_graph_type;
    register u32 xbits __asm__("$2");     /* A separate x result preserves the original v0 allocation. */
    register u32 ybits __asm__("$3");     /* The original shifts the masked y result in place. */
    __asm__("" : "=r"(type) : "0"(type)); /* The original materializes the environment address. */
    if ((u32)(*type - 1) >= 2) {
        ybits = (y & 0x7ff) << 11;
        xbits = x & 0x7ff;
    } else {
        ybits = (y & 0xfff) << 12;
        xbits = x & 0xfff;
    }
    xbits |= PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_OFFSET);
    return ybits | xbits;
}
