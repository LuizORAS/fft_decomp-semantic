#include "psx/libc.h"
#include "psx/libetc.h"
#include "psx/libgpu.h"

/* LIBGPU, retail 0x80026d10-0x80026e7c. Restores DMA/GPU state after a timeout. */
int get_alarm(void) {
    register int head __asm__("$5");
    int mask;
    int tail;
    psyq_gpu_operation_t* operation;
    volatile u32* control;
    register u32 discarded __asm__("$2");
    if (VSync(-1) > g_psyq_gpu_sync_deadline || g_psyq_gpu_sync_poll_count++ > 0x000f0000) {
        control = g_psyq_gpu_gp1_port;
        __asm__("" : "=r"(control) : "0"(control)); /* Preserve the original MMIO pointer before diagnostic setup. */
        discarded = *control;
        head = g_psyq_gpu_queue_head;
        printf(g_psyq_gpu_timeout_state_format, (head - g_psyq_gpu_queue_tail) & 63, *control, *g_psyq_gpu_dma_chcr,
            *g_psyq_gpu_dma_madr);
        operation = &g_psyq_gpu_last_operation;
        __asm__("" : "=r"(operation) : "0"(operation)); /* The original materializes this saved-operation pointer. */
        printf(g_psyq_gpu_timeout_operation_format, *operation, g_psyq_gpu_last_source, g_psyq_gpu_last_argument);
        mask = SetIntrMask(0);
        g_psyq_gpu_queue_tail = 0;
        tail = g_psyq_gpu_queue_tail;
        g_psyq_gpu_reset_interrupt_mask = mask;
        g_psyq_gpu_queue_head = tail;
        *g_psyq_gpu_dma_chcr = (PSYQ_GPU_DMA_MODE_LINKED_LIST | PSYQ_GPU_DMA_TO_DEVICE);
        *g_psyq_gpu_dma_dpcr |= PSYQ_GPU_DMA_GPU_ENABLE;
        *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_ACK_IRQ);
        *g_psyq_gpu_gp1_port = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_GP1_RESET_BUFFER);
        SetIntrMask(g_psyq_gpu_reset_interrupt_mask);
        return -1;
    }
    return 0;
}
