/* SCUS_942.21 0x80018858..0x80018aeb. */
#include "psx/libspu.h"

#include "psx/libc.h"

s32 _spu_init(s32 hot) {
    u32 wait;
    s32 i;
    /* The pin retains the retail v0 return value across the hot/cold paths. */
    register s32 result __asm__("$2");
    volatile psyq_spu_voice_t* voice;
    *g_psyq_spu_dma_priority_register |= 0xb0000;
    _spu_transMode = 0;
    _spu_addrMode = 0;
    _spu_tsa = 0;
    _spu_RXX->main_volume_left = 0;
    _spu_RXX->main_volume_right = 0;
    _spu_RXX->control = 0;
    _spu_Fw1ts();
    _spu_RXX->main_volume_left = 0;
    _spu_RXX->main_volume_right = 0;
    wait = 0;
    while (_spu_RXX->status & PSYQ_SPU_STATUS_MASK) {
        if (++wait > PSYQ_SPU_POLL_LIMIT) {
            printf(g_psyq_spu_timeout_format, g_psyq_spu_init_timeout_name);
            break;
        }
    }
    _spu_mem_mode = 2;
    _spu_mem_mode_plus = 3;
    _spu_mem_mode_unit = 8;
    _spu_mem_mode_unitM = 7;
    _spu_RXX->transfer_control = 4;
    _spu_RXX->reverb_depth_left = 0;
    _spu_RXX->reverb_depth_right = 0;
    _spu_RXX->key_off_low = 0xffff;
    _spu_RXX->key_off_high = 0xffff;
    _spu_RXX->reverb_low = 0;
    _spu_RXX->reverb_high = 0;
    for (i = 0; i < 10; i++) {
        _spu_RQ[i] = 0;
    }
    result = 0;
    if (!hot) {
        _spu_tsa = 0x200;
        _spu_RXX->pitch_lfo_low = 0;
        _spu_RXX->pitch_lfo_high = 0;
        _spu_RXX->noise_low = 0;
        _spu_RXX->noise_high = 0;
        _spu_RXX->cd_volume_left = 0;
        _spu_RXX->cd_volume_right = 0;
        _spu_RXX->external_volume_left = 0;
        _spu_RXX->external_volume_right = 0;
        _spu_writeByIO(g_psyq_spu_init_fifo_data, 16);
        voice = _spu_RXX->voices;
        for (i = 0; i < 24; i++, voice++) {
            voice->volume_left = 0;
            voice->volume_right = 0;
            voice->pitch = 0x3fff;
            voice->start_address = 0x200;
            voice->adsr1 = 0;
            voice->adsr2 = 0;
            /* Otherwise reorg moves the voice increment into a load-delay slot. */
            __asm__ volatile("");
        }
        _spu_RXX->key_on_low = 0xffff;
        _spu_RXX->key_on_high = 0xff;
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_RXX->key_off_low = 0xffff;
        _spu_RXX->key_off_high = 0xff;
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        result = 0;
    }
    {
        volatile psyq_spu_registers_t* final_registers = _spu_RXX;
        _spu_inTransfer = 1;
        final_registers->control = 0xc000;
        /* Without the barrier the control store moves after callback clearing. */
        __asm__ volatile("");
        _spu_transferCallback = 0;
        _spu_IRQCallback = 0;
    }
    return result;
}
