/* LIBGPU 800257c8-80025824. */
#include "psx/libgpu.h"

u32 get_mode(int dfe, int dtd, int tpage) {
    u8* type = &g_psyq_gpu_graph_type;
    u32 command;
    register u32 flags __asm__("$2");     /* Reusing the argument register changes the original mask operations. */
    __asm__("" : "=r"(type) : "0"(type)); /* The original materializes the environment address. */
    if ((u32)(*type - 1) < 2) {
        command = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MODE);
        if (dtd) {
            command |= 0x800;
        }
        flags = tpage & 0x27ff;
        if (dfe) {
            flags |= 0x1000;
        }
    } else {
        command = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MODE);
        if (dtd) {
            command |= 0x200;
        }
        flags = tpage & 0x9ff;
        if (dfe) {
            flags |= 0x400;
        }
    }
    return command | flags;
}
