/* SCUS_942.21 0x80018aec..0x80018cb7. */
#include "psx/libspu.h"

#include "psx/libc.h"

void _spu_writeByIO(const u16* source, u32 size) {
    /* These pins retain the retail status load and saved-register allocation. */
    register u16 initial_status __asm__("$5");
    u16 status;
    register u32 remaining __asm__("$17") = size;
    s32 chunk;
    s32 i;
    u32 wait;
    volatile psyq_spu_registers_t* registers;
    {
        register volatile psyq_spu_registers_t* initial_registers __asm__("$2") = _spu_RXX;
        initial_status = initial_registers->status;
        status = initial_status & PSYQ_SPU_STATUS_MASK;
        initial_registers->transfer_address = _spu_tsa;
    }
    _spu_Fw1ts();
    while (remaining != 0) {
        chunk = remaining;
        if (remaining > PSYQ_SPU_FIFO_CHUNK_BYTES)
            chunk = PSYQ_SPU_FIFO_CHUNK_BYTES;
        for (i = 0; i < chunk; i += 2) {
            registers = _spu_RXX;
            registers->transfer_fifo = *source++;
        }
        {
            volatile psyq_spu_registers_t* control_registers = _spu_RXX;
            /* The pin preserves the retail control value in a0. */
            register u16 control __asm__("$4") = control_registers->control;
            register u32 masked __asm__("$2") = control & PSYQ_SPU_TRANSFER_MODE_CLEAR;
            control = masked | PSYQ_SPU_TRANSFER_MODE_IO;
            control_registers->control = control;
        }
        _spu_Fw1ts();
        wait = 0;
        while (_spu_RXX->status & PSYQ_SPU_STATUS_TRANSFER_BUSY) {
            if (++wait > PSYQ_SPU_POLL_LIMIT) {
                printf(g_psyq_spu_timeout_format, g_psyq_spu_fifo_ready_timeout_name);
                break;
            }
        }
        /* The tie retains the count update in the first settling-call delay slot. */
        __asm__ volatile("" : "=r"(remaining) : "0"(remaining));
        remaining -= chunk;
        _spu_Fw1ts();
        _spu_Fw1ts();
    }
    wait = 0;
    {
        /* The pin preserves the retail control value in a0. */
        register u16 control __asm__("$4") = _spu_RXX->control;
        _spu_RXX->control = control & PSYQ_SPU_TRANSFER_MODE_CLEAR;
    }
    while ((_spu_RXX->status & PSYQ_SPU_STATUS_MASK) != status) {
        if (++wait > PSYQ_SPU_POLL_LIMIT) {
            printf(g_psyq_spu_timeout_format, g_psyq_spu_status_restore_timeout_name);
            break;
        }
    }
}
