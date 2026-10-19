#include "psx/libgpu.h"

/* The dispatch table's sync, behind DrawSync: mode 0 runs the queue until it is empty and waits for the DMA
 * and the GPU (-1 on a timeout); any other mode only starts the queue and returns how many entries wait, at
 * least 1 while the GPU is busy. */
int _sync(int mode) {
    int count;
    if (!mode) {
        set_alarm();
        while (_qin != _qout) {
            _exeque();
            if (get_alarm())
                return -1;
        }
        while ((*g_psyq_gpu_dma_chcr & PSYQ_DMA_BUSY) || !(*g_psyq_gpu_gp1_port & PSYQ_GPU_STATUS_READY_COMMAND)) {
            if (get_alarm())
                return -1;
        }
        return 0;
    }
    count = (_qin - _qout) & PSYQ_GPU_QUEUE_INDEX_MASK;
    if (count)
        _exeque();
    if ((*g_psyq_gpu_dma_chcr & PSYQ_DMA_BUSY) || !(*g_psyq_gpu_gp1_port & PSYQ_GPU_STATUS_READY_COMMAND)) {
        return count ? count : 1;
    }
    return count;
}
