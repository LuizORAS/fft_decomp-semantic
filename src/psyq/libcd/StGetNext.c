/* SCUS_942.21 0x80021208..0x800212cb. */
#include "psx/libcd.h"
u32 StGetNext(u32** frame_data, psyq_cd_ring_record_t** header) {
    psyq_cd_ring_record_t* record = &g_psyq_cd_stream_ring[g_psyq_cd_stream_read_index];
    if (*(volatile u16*)&record->status == StREWIND) {
        g_psyq_cd_stream_read_index = 0;
        if (g_psyq_cd_stream_end_frame)
            record->status = StFREE;
        record = &g_psyq_cd_stream_ring[g_psyq_cd_stream_read_index];
    }
    if (*(volatile u16*)&record->status != StCOMPLETE)
        return 1;
    /* The addressed halfword keeps the lock store before the payload address calculation. */
    *&record->status = StLOCK;
    *frame_data = (u32*)(g_psyq_cd_stream_ring + g_psyq_cd_stream_ring_sectors)
        + g_psyq_cd_stream_read_index * PSYQ_CD_STREAM_PAYLOAD_WORDS;
    *header = record;
    return 0;
}
