/* SCUS_942.21 0x80021c90..0x80021e4b. */
#include "psx/libc.h"
#include "psx/libcd.h"
enum {
    CD_DMA_TRANSFER_ACTIVE = 0x01000000,
    CD_DMA_BUSY_POLL_LIMIT = 0x10000,
    CD_DMA_IRQ_ENABLE_REQUEST = 1,
    CD_DMA_IRQ_ENABLE_BYTE = 2,
    CD_DMA_PRIORITY_FIELD_BITS = 4,
    CD_DMA_PRIORITY_ENABLE_BIT = 3,
    CD_STATUS_DATA_READY = 0x40,
};

#define CD_DMA_CHANNELS ((volatile psyq_cd_dma_channel_t*)0x1f801080)
void dma_execute(s32 channel, u32* destination, u32 blocks, u32 words, u32 control, u8 enable_irq, s32 unused) {
    s32 polls = 0;
    /* Volatile keeps the original stack stores of both MMIO readbacks. */
    volatile u32 observed_register;
    volatile u32* dma;
    volatile psyq_cd_dma_interrupt_register_t* interrupt_control;
    /* Removing this pin changes the original IRQ-byte register allocation. */
    register u32 enabled_channels __asm__("$4");
    u32 priority_mask;
    /* Removing this pin changes the original priority-register allocation. */
    register s32 shift __asm__("$6");
    while (CD_DMA_CHANNELS[channel].control & CD_DMA_TRANSFER_ACTIVE) {
        if (polls == CD_DMA_BUSY_POLL_LIMIT) {
            printf(g_psyq_cd_dma_timeout_format, CD_DMA_CHANNELS[channel].control);
            break;
        }
        polls++;
    }
    {
        u32 irq_enable_value;
        /* Removing this pin changes the shared IRQ-mask shift. */
        register u32 channel_mask __asm__("$2");
        /* Assigning the comparison constant here preserves the original byte narrowing. */
        if (enable_irq == (irq_enable_value = CD_DMA_IRQ_ENABLE_REQUEST)) {
            interrupt_control = g_psyq_cd_stream_dma_interrupt_register;
            enabled_channels = interrupt_control->bytes[CD_DMA_IRQ_ENABLE_BYTE];
            channel_mask = irq_enable_value << channel;
            /* The tie and constant input retain one shift and the original comparison register. */
            __asm__("" : "=r"(channel_mask) : "0"(channel_mask), "r"(irq_enable_value));
            enabled_channels |= channel_mask;
        } else {
            interrupt_control = g_psyq_cd_stream_dma_interrupt_register;
            enabled_channels = interrupt_control->bytes[CD_DMA_IRQ_ENABLE_BYTE];
            channel_mask = irq_enable_value << channel;
            enabled_channels &= ~channel_mask;
        }
        interrupt_control->bytes[CD_DMA_IRQ_ENABLE_BYTE] = enabled_channels;
    }
    observed_register = g_psyq_cd_stream_dma_interrupt_register->word;
    /* Removing this barrier changes the readback and following register order. */
    __asm__ volatile("" ::: "memory");
    shift = channel * CD_DMA_PRIORITY_FIELD_BITS;
    /* The tie keeps the original shift and add sequence for the enable bit. */
    __asm__("" : "=r"(shift) : "0"(shift));
    shift += CD_DMA_PRIORITY_ENABLE_BIT;
    priority_mask = 1U;
    priority_mask <<= shift;
    /* Typed field stores change the original consecutive MADR, BCR and CHCR access sequence. */
    dma = &CD_DMA_CHANNELS[channel].address;
    {
        volatile u32* priority = g_psyq_cd_stream_dma_priority_register;
        /* Removing this pin changes the original DPCR register allocation. */
        register u32 previous __asm__("$6") = *priority;
        previous |= priority_mask;
        *priority = previous;
    }
    *dma++ = (u32)destination;
    *dma++ = (blocks << 16) | words;
    while (!(*g_psyq_cd_stream_index_register & CD_STATUS_DATA_READY)) { }
    *dma = control;
    observed_register = *dma;
}
