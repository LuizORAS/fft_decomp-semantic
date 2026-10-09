/* SCUS_942.21 0x80018e44..0x800190d3. */
#include "psx/libspu.h"

s32 _spu_t(s32 operation, ...) {
    u32* arguments;
    u32 wait;
    u32 dma_control_word;
    u16 expected;
    volatile psyq_spu_registers_t* registers;
    /* This linked ABI consumes 32-bit words from the variadic register-save area. */
    arguments = (u32*)&operation + 1;
    switch (operation) {
    case PSYQ_SPU_DMA_SET_ADDRESS: { /* Unpinned address/shift operands change retail register allocation. */
        register u32 address __asm__("$4") = *arguments;
        register u32 shift __asm__("$2") = _spu_mem_mode_plus;
        volatile psyq_spu_registers_t* address_registers = _spu_RXX;
        u32 units = address >> shift;
        _spu_tsa = units;
        address_registers->transfer_address = units;
    } break;
    case PSYQ_SPU_DMA_WRITE:
        g_psyq_spu_dma_direction = 0;
        registers = _spu_RXX;
        expected = _spu_tsa;
        wait = 0;
        while (registers->transfer_address != expected) {
            if (++wait > PSYQ_SPU_POLL_LIMIT) {
                return -2;
            }
        }
        { /* The control and mask pins preserve the retail a0/v0 update registers. */
            volatile psyq_spu_registers_t* control_registers = _spu_RXX;
            register u16 control __asm__("$4") = control_registers->control;
            register u32 masked __asm__("$2") = control & PSYQ_SPU_TRANSFER_MODE_CLEAR;
            control = masked | PSYQ_SPU_TRANSFER_MODE_DMA_WRITE;
            control_registers->control = control;
        }
        break;
    case PSYQ_SPU_DMA_READ:
        g_psyq_spu_dma_direction = 1;
        registers = _spu_RXX;
        expected = _spu_tsa;
        wait = 0;
        while (registers->transfer_address != expected) {
            if (++wait > PSYQ_SPU_POLL_LIMIT) {
                return -2;
            }
        }
        { /* The control-value pin preserves the retail a0 read/modify/write register. */
            volatile psyq_spu_registers_t* control_registers = _spu_RXX;
            register u16 control __asm__("$4") = control_registers->control;
            control |= PSYQ_SPU_TRANSFER_MODE_DMA_READ;
            control_registers->control = control;
        }
        break;
    case PSYQ_SPU_DMA_START_TRANSFER:
        expected = g_psyq_spu_dma_direction == 1 ? PSYQ_SPU_TRANSFER_MODE_DMA_READ : PSYQ_SPU_TRANSFER_MODE_DMA_WRITE;
        registers = _spu_RXX;
        wait = 0;
        while ((registers->control & PSYQ_SPU_TRANSFER_MODE_MASK) != expected) {
            if (++wait > PSYQ_SPU_POLL_LIMIT) {
                return -2;
            }
        }
        if (g_psyq_spu_dma_direction == 1) {
            arguments++;
            _spu_FsetDelayW();
            dma_control_word = 0x01000000;
        } else {
            arguments++;
            _spu_FsetDelayR();
            dma_control_word = 0x01000000;
        }
        {
            /* An unpinned buffer changes the retail a0 argument/store register. */
            register void* buffer __asm__("$4") = (void*)arguments[-1];
            g_psyq_spu_dma_buffer = buffer;
        }
        {
            /* The byte-count and buffer pins preserve retail reuse of a0 across the DMA address load. */
            register u32 bytes __asm__("$4") = *arguments;
            volatile u32* dma_address = g_psyq_spu_dma_address_register;
            u32 blocks = bytes >> 6;
            u32 partial = (bytes & 0x3f) != 0;
            register void* buffer __asm__("$4") = g_psyq_spu_dma_buffer;
            blocks += partial;
            g_psyq_spu_dma_blocks = blocks;
            *dma_address = (u32)buffer;
        }
        *g_psyq_spu_dma_block_register = (g_psyq_spu_dma_blocks << 16) | 0x10;
        dma_control_word |= 0x201;
        if (g_psyq_spu_dma_direction == 1)
            dma_control_word = 0x01000200;
        *g_psyq_spu_dma_control_register = dma_control_word;
        break;
    }
    return 0;
}
