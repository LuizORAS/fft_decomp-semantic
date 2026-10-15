/* SCUS_942.21 0x80018aec..0x80018cb7. */
#include "psx/libspu.h"

#include "psx/libc.h"

/* Write `size` bytes from `source` to SPU RAM at the transfer address through the SPU's data FIFO, 64 bytes
 * at a time in programmed I/O mode, waiting after each chunk for the SPU to finish; then leave the transfer
 * mode and wait for the status to settle. Each wait prints a message on a timeout. _spu_Fw uses it in I/O
 * mode and _spu_init for its start-up block. */
void _spu_writeByIO(const u16* source, u32 size) {
    u16 status;
    u16 control;
    s32 chunk;
    s32 i;
    u32 wait;
    const u16* data;
    status = _spu_RXX->rxx.status & PSYQ_SPU_STATUS_MASK;
    data = source;
    _spu_RXX->rxx.transfer_address = _spu_tsa;
    _spu_Fw1ts();
    while (size != 0) {
        chunk = size > PSYQ_SPU_FIFO_CHUNK_BYTES ? PSYQ_SPU_FIFO_CHUNK_BYTES : size;
        for (i = 0; i < chunk; i += 2) {
            _spu_RXX->rxx.transfer_fifo = *data++;
        }
        control = _spu_RXX->rxx.control;
        control = (control & PSYQ_SPU_TRANSFER_MODE_CLEAR) | PSYQ_SPU_TRANSFER_MODE_IO;
        _spu_RXX->rxx.control = control;
        _spu_Fw1ts();
        wait = 0;
        while (_spu_RXX->rxx.status & PSYQ_SPU_STATUS_TRANSFER_BUSY) {
            if (++wait > PSYQ_SPU_POLL_LIMIT) {
                printf(g_psyq_spu_timeout_format, g_psyq_spu_fifo_ready_timeout_name);
                break;
            }
        }
        _spu_Fw1ts();
        _spu_Fw1ts();
        size -= chunk;
    }
    control = _spu_RXX->rxx.control;
    control &= PSYQ_SPU_TRANSFER_MODE_CLEAR;
    _spu_RXX->rxx.control = control;
    wait = 0;
    while ((_spu_RXX->rxx.status & PSYQ_SPU_STATUS_MASK) != status) {
        if (++wait > PSYQ_SPU_POLL_LIMIT) {
            printf(g_psyq_spu_timeout_format, g_psyq_spu_status_restore_timeout_name);
            break;
        }
    }
}
