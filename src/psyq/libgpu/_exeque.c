#include "psx/libetc.h"
#include "psx/libgpu.h"

/* LIBGPU, retail 0x8002675c-0x80026a58. DMA callback processes queued operations and completion. */
int _exeque(void) {
    extern volatile psyq_gpu_environment_t g_psyq_gpu_environment; /* An unqualified RAM view changes the native
                                                                      completion-store and callback-load sequence. */
    if (*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY)
        return 1;
    g_psyq_gpu_queue_interrupt_mask = SetIntrMask(0);
    while (_qin != _qout && !(*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY)) {
        if (((_qout + 1) & PSYQ_GPU_QUEUE_INDEX_MASK) == _qin && !g_psyq_gpu_environment.draw_sync_callback)
            DMACallback(PSYQ_GPU_DMA_GPU_CHANNEL, 0);
        while (!(*g_psyq_gpu_gp1_port & PSYQ_GPU_STATUS_READY_COMMAND))
            ;
        _que[_qout].request.operation(_que[_qout].request.source, _que[_qout].request.argument);
        _qlog.operation = _que[_qout].request.operation;
        _qlog.source = _que[_qout].request.source;
        _qlog.argument = _que[_qout].request.argument;
        _qout = (_qout + 1) & PSYQ_GPU_QUEUE_INDEX_MASK;
    }
    SetIntrMask(g_psyq_gpu_queue_interrupt_mask);
    if (_qin == _qout && !(*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY) && g_psyq_gpu_environment.completion_pending
        && g_psyq_gpu_environment.draw_sync_callback) {
        g_psyq_gpu_environment.completion_pending = 0;
        g_psyq_gpu_environment.draw_sync_callback();
    }
    return (_qin - _qout) & PSYQ_GPU_QUEUE_INDEX_MASK;
}
