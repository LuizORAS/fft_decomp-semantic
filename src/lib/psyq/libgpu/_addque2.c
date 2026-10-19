#include "psx/libetc.h"
#include "psx/libgpu.h"

/* Run a GPU operation now or queue it. While the 64-entry queue is full it runs queued work (_exeque),
 * giving up with -1 on a timeout. With interrupts masked: when queueing is off, or the queue is empty with no
 * DMA busy and no DrawSync callback, it waits for the GPU and runs `operation` at once; otherwise it stores the
 * operation (copying `copy_size` bytes of `source` into the entry), sets _exeque as the GPU DMA callback and
 * starts the queue. Returns 0, or the number of entries waiting. */
int _addque2(psyq_gpu_operation_t operation, void* source, int copy_size, u32 argument) {
    int i;
    int interrupt_mask;
    /* Ordinary field accesses fold the store address and move queue reads ahead of it. */
    volatile s32* completion_pending;
    set_alarm();
    while (((_qin + 1) & PSYQ_GPU_QUEUE_INDEX_MASK) == _qout) {
        if (get_alarm())
            return -1;
        _exeque();
    }
    interrupt_mask = SetIntrMask(0);
    completion_pending = &g_psyq_gpu_environment.completion_pending;
    *completion_pending = 1;
    g_psyq_gpu_enqueue_interrupt_mask = interrupt_mask;
    if (!(*(volatile u8*)&g_psyq_gpu_environment.queue_enabled)
        || (_qin == _qout && !(*g_psyq_gpu_dma_chcr & PSYQ_DMA_BUSY)
            && !(*(volatile psyq_interrupt_callback_t*)&g_psyq_gpu_environment.draw_sync_callback))) {
        while (!(*g_psyq_gpu_gp1_port & PSYQ_GPU_STATUS_READY_COMMAND))
            ;
        operation(source, argument);
        _qlog.operation = operation;
        _qlog.source = source;
        _qlog.argument = argument;
        SetIntrMask(g_psyq_gpu_enqueue_interrupt_mask);
        return 0;
    }
    DMACallback(PSYQ_DMA_GPU_CHANNEL, _exeque);
    if (copy_size) {
        for (i = 0; i < copy_size / 4; i++)
            _que[_qin].packet_words[i] = ((u32*)source)[i];
        _que[_qin].request.source = _que[_qin].packet_words;
    } else
        _que[_qin].request.source = source;
    _que[_qin].request.argument = argument;
    _que[_qin].request.operation = operation;
    _qin = (_qin + 1) & PSYQ_GPU_QUEUE_INDEX_MASK;
    SetIntrMask(g_psyq_gpu_enqueue_interrupt_mask);
    _exeque();
    return (_qin - _qout) & PSYQ_GPU_QUEUE_INDEX_MASK;
}
