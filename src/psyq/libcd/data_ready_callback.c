/* SCUS_942.21 0x80020f94..0x80021023. */
#include "psx/libcd.h"
void data_ready_callback(void) {
    StHEADER* record = &g_psyq_cd_stream_ring[g_psyq_cd_stream_frame_start_index];
    *(s16*)&record->id = StCOMPLETE;
    g_psyq_cd_backloc.position = record->loc;
    g_psyq_cd_backloc.frame = record->frameCount;
    g_psyq_cd_stream_frame_start_index = g_psyq_cd_stream_write_index;
    if (g_psyq_cd_stream_frame_callback)
        g_psyq_cd_stream_frame_callback();
    g_psyq_cd_stream_frame_dma_pending = 0;
}
