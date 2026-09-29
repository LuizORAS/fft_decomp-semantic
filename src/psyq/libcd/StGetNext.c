/* SCUS_942.21 0x80021208..0x800212cb. */
#include "psx/libcd.h"
s32 StGetNext(void** frame_data, void** header) {
    volatile psyq_cd_ring_record_t* record = &g_psyq_cd_stream_ring[g_psyq_cd_stream_read_index];
    if (record->status == StREWIND) {
        g_psyq_cd_stream_read_index = 0;
        if (g_psyq_cd_stream_end_frame)
            record->status = StFREE;
        record = &g_psyq_cd_stream_ring[g_psyq_cd_stream_read_index];
    }
    if (record->status != StCOMPLETE)
        return 1;
    record->status = StLOCK;
    /* Keep the descriptor transition before preparing the output pointers. */
    __asm__ volatile("");
    *frame_data = (u8*)(g_psyq_cd_stream_ring + g_psyq_cd_stream_ring_sectors)
        + g_psyq_cd_stream_read_index * PSYQ_CD_STREAM_PAYLOAD_BYTES;
    *header = (void*)record;
    return 0;
}
