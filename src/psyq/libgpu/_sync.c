#include "psx/libgpu.h"

/* LIBGPU, retail 0x80026b94-0x80026cdc. Blocking/nonblocking DMA and queue synchronization. */
int _sync(int mode) {
    int count;
    if (!mode) {
        set_alarm();
        while (g_psyq_gpu_queue_head != g_psyq_gpu_queue_tail) {
            _exeque();
            if (get_alarm())
                return -1;
        }
        while ((*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY) || !(*g_psyq_gpu_gp1_port & PSYQ_GPU_STATUS_READY_COMMAND)) {
            if (get_alarm())
                return -1;
        }
        return 0;
    }
    count = (g_psyq_gpu_queue_head - g_psyq_gpu_queue_tail) & 63;
    if (count)
        _exeque();
    if ((*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY) || !(*g_psyq_gpu_gp1_port & PSYQ_GPU_STATUS_READY_COMMAND)) {
        return count ? count : 1;
    }
    return count;
}
