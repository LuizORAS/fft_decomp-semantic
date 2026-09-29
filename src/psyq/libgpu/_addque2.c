#include "psx/libetc.h"
#include "psx/libgpu.h"

/* LIBGPU, retail 0x80026478-0x8002675c. Immediate or copied/deferred GPU queue operation. */
int _addque2(psyq_gpu_operation_t operation, void* source, int copy_size, u32 argument) {
    extern volatile int g_psyq_gpu_queue_head; /* Interrupt-visible head; retail performs consecutive distinct loads. */
    int mask;
    int* completion_pending;
    int index;
    u32* words;
    psyq_gpu_operation_snapshot_t* last;
    int restore_mask;
    /* The ready mask and copied-packet base retain the retail wait/copy registers. */
    register u32 ready __asm__("$4");
    set_alarm();
    while (((g_psyq_gpu_queue_head + 1) & 63) == g_psyq_gpu_queue_tail) {
        if (get_alarm())
            return -1;
        _exeque();
    }
    mask = SetIntrMask(0);
    completion_pending = &g_psyq_gpu_completion_pending;
    __asm__(""
        : "=r"(completion_pending)
        : "0"(completion_pending)); /* Retail materializes the completion_pending flag's address. */
    *completion_pending = 1;
    g_psyq_gpu_enqueue_interrupt_mask = mask;
    ready = PSYQ_GPU_STATUS_READY_COMMAND;
    if (!g_psyq_gpu_queue_enabled
        || (g_psyq_gpu_queue_head == g_psyq_gpu_queue_tail && !(*g_psyq_gpu_dma_chcr & PSYQ_GPU_DMA_BUSY)
            && !g_psyq_gpu_draw_sync_callback)) {
        volatile u32* status = g_psyq_gpu_gp1_port;
        u32 sample;
        do {
            sample = *status;
            __asm__("" : "=r"(sample) : "0"(sample)); /* Preserve sample-then-mask operand order. */
            sample &= ready;
        } while (!sample);
        operation(source, argument);
        restore_mask = g_psyq_gpu_enqueue_interrupt_mask;
        __asm__(""
            : "=r"(restore_mask)
            : "0"(restore_mask)
            : "$2"); /* Retail reads the restore mask before materializing the snapshot pointer. */
        last = (psyq_gpu_operation_snapshot_t*)&g_psyq_gpu_last_operation;
        __asm__("" : "=r"(last) : "0"(last)); /* Retail materializes the saved-operation pointer. */
        last->operation = operation;
        g_psyq_gpu_last_source = source;
        g_psyq_gpu_last_argument = argument;
        SetIntrMask(restore_mask);
        return 0;
    }
    DMACallback(PSYQ_GPU_DMA_GPU_CHANNEL, _exeque);
    index = 0;
    if (copy_size) {
        register u32* payload __asm__("$8") = g_psyq_gpu_operation_queue[0].packet_words;
        psyq_gpu_queue_packet_view_t* packets;
        __asm__(""
            : "=r"(payload)
            : "0"(payload)
            : "$7"); /* Retain payload-base setup before the source register copy. */
        words = source;
        __asm__(""
            : "=r"(words)
            : "0"(words), "r"(payload)); /* Retail prepares the payload base before copying the source pointer. */
        /* A view beginning at the packet member preserves the retail base and 96-byte stride. */
        packets = (psyq_gpu_queue_packet_view_t*)payload;
        while (index < copy_size / 4) {
            packets[g_psyq_gpu_queue_head].packet_words[index++] = *words++;
            __asm__ volatile("" ::: "memory"); /* Retail stores each copied word before recomputing its loop bound. */
        }
        g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].source
            = g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].packet_words;
    } else
        g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].source = source;
    g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].argument = argument;
    g_psyq_gpu_operation_queue[g_psyq_gpu_queue_head].operation = operation;
    g_psyq_gpu_queue_head = (g_psyq_gpu_queue_head + 1) & 63;
    SetIntrMask(g_psyq_gpu_enqueue_interrupt_mask);
    _exeque();
    return (g_psyq_gpu_queue_head - g_psyq_gpu_queue_tail) & 63;
}
