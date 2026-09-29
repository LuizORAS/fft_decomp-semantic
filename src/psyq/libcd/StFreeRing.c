/* SCUS_942.21 0x8002110c..0x800211c7. */
#include "psx/libcd.h"
s32 StFreeRing(void* frame_data) {
    s32 ring_sectors = g_psyq_cd_stream_ring_sectors;
    psyq_cd_ring_record_t* ring = g_psyq_cd_stream_ring;
    /* Integer address sums preserve operand order; typed pointer additions swap it. */
    s32 first_sector
        = ((u32*)frame_data - (u32*)((ring_sectors * sizeof(*ring)) + (u32)ring)) / PSYQ_CD_STREAM_PAYLOAD_WORDS;
    psyq_cd_ring_record_t* record = (psyq_cd_ring_record_t*)((first_sector * sizeof(*ring)) + (u32)ring);
    s32 frame_sectors = record->sector_count;
    s32 sector_index;
    if (*(s16*)&record->status != StLOCK)
        return 1;
    /* Retail compares the unsigned sector count through its signed halfword view. */
    for (sector_index = 0; sector_index < (s16)frame_sectors; sector_index++) {
        psyq_cd_ring_record_t* ring = g_psyq_cd_stream_ring;
        ring[sector_index + first_sector].status = StFREE;
    }
    g_psyq_cd_stream_read_index = sector_index + first_sector;
    return 0;
}
