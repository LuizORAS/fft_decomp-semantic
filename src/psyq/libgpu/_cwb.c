/* LIBGPU 80026388-800263d8. */
#include "psx/libgpu.h"

/* Write `count` command words from `commands` to GP0 by the CPU. DrawPrim sends one primitive this way. */
int _cwb(u32* commands, int count) {
    int unused[2]; /* The original leaf reserves eight stack bytes. */
    int remaining = count - 1;
    *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_DMA_DIRECTION);
    if (count) {
        do {
            *g_psyq_gpu_gp0_port = *commands++;
        } while (remaining-- != 0);
    }
    return 0;
}
