/* SCUS_942.21 0x80020f94..0x80021023. */
#include "psx/libcd.h"
void data_ready_callback(void) {
    psyq_cd_ring_record_t* record = &g_psyq_cd_stream_ring[g_psyq_cd_stream_frame_start_index];
    record->status = StCOMPLETE;
    g_psyq_cd_backloc.position = record->position;
    {
        u32 frame = record->frame_count; /* The original loads the header frame before either publication store. */
        __asm__("" : "=r"(frame) : "0"(frame));
        g_psyq_cd_backloc.frame = frame;
        g_psyq_cd_stream_frame_start_index = g_psyq_cd_stream_write_index;
        if (g_psyq_cd_stream_frame_callback)
            g_psyq_cd_stream_frame_callback();
    }
    g_psyq_cd_stream_frame_dma_pending = 0;
}
