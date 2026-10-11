#include "psx/libetc.h"
#include "psx/libgpu.h"

/* LIBGPU, retail 0x80026478-0x8002675c. Immediate or copied/deferred GPU queue operation. */
int _addque2(psyq_gpu_operation_t operation, void* source, int copy_size, u32 argument) {
    extern volatile psyq_gpu_environment_t
        g_psyq_gpu_environment; /* An unqualified RAM view folds the native queue-state address sequence. */
    int i;
    set_alarm();
    while (((g_psyq_gpu_queue_head + 1) & 63) == g_psyq_gpu_queue_tail) {
        if (get_alarm())
            return -1;
        _exeque();
    }
    g_psyq_gpu_enqueue_interrupt_mask = SetIntrMask(0);
    g_psyq_gpu_environment.completion_pending = 1;
    if (!g_psyq_gpu_environment.queue_enabled
        || (g_psyq_gpu_queue_head == g_psyq_gpu_queue_tail && !(*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY)
            && !g_psyq_gpu_environment.draw_sync_callback)) {
        while (!(*g_psyq_gpu_gp1_port & PSYQ_GPU_STATUS_READY_COMMAND))
            ;
        operation(source, argument);
        g_psyq_gpu_last_operation.operation = operation;
        g_psyq_gpu_last_operation.source = source;
        g_psyq_gpu_last_operation.argument = argument;
        SetIntrMask(g_psyq_gpu_enqueue_interrupt_mask);
        return 0;
    }
    DMACallback(PSYQ_GPU_DMA_GPU_CHANNEL, _exeque);
    if (copy_size) {
        for (i = 0; i < copy_size / 4; i++)
            g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].packet_words[i] = ((u32*)source)[i];
        g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].request.source
            = g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].packet_words;
    } else
        g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].request.source = source;
    g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].request.argument = argument;
    g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].request.operation = operation;
    g_psyq_gpu_queue_head = (g_psyq_gpu_queue_head + 1) & 63;
    SetIntrMask(g_psyq_gpu_enqueue_interrupt_mask);
    _exeque();
    return (g_psyq_gpu_queue_head - g_psyq_gpu_queue_tail) & 63;
}
