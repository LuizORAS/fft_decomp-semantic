/* SCUS_942.21 0x80021208..0x800212cb. */
#include "psx/libcd.h"

/* Lock the next complete frame and return its data and its header record (0), or 1 when none is ready; a
 * rewind record sends the read index back to the start of the ring. */
s32 StGetNext(void** frame_data, void** header) {
    psyq_cd_ring_record_t* record = &g_psyq_cd_stream_ring[g_psyq_cd_stream_read_index];
    if (*(volatile u16*)&record->status == StREWIND) {
        g_psyq_cd_stream_read_index = 0;
        if (g_psyq_cd_stream_end_frame)
            record->status = StFREE;
        record = &g_psyq_cd_stream_ring[g_psyq_cd_stream_read_index];
    }
    if (*(volatile u16*)&record->status != StCOMPLETE)
        return 1;
    *&record->status = StLOCK;
    *frame_data = (u8*)(g_psyq_cd_stream_ring + g_psyq_cd_stream_ring_sectors)
        + g_psyq_cd_stream_read_index * PSYQ_CD_STREAM_PAYLOAD_BYTES;
    *header = record;
    return 0;
}
