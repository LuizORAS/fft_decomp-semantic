/* SCUS_942.21 0x80021e4c..0x80021f13. */
#include "psx/libcd.h"

void StRingStatus(s16* free_sectors, s16* read_sectors) {
    s32 index = g_psyq_cd_stream_ring_sectors;
    /* Without this pin GCC uses v0 for the final descriptor address. */
    register s32 offset __asm__("$3");
    *free_sectors = 0;
    *read_sectors = (u16)g_psyq_cd_stream_write_index - (u16)g_psyq_cd_stream_read_index;
    if (*read_sectors < 0) {
        index--;
        if (index >= 0) {
            /* The tied use retains separate descriptor and induction registers. */
            psyq_cd_ring_record_t* scan = &g_psyq_cd_stream_ring[index];
            do {
                psyq_cd_ring_record_t* record = scan;
                /* Preserve the separate scan induction and descriptor values. */
                __asm__("" : "=r"(scan) : "0"(scan));
                if (record->status == StREWIND)
                    break;
                index--;
                scan = record - 1;
            } while (index >= 0);
        }
        /* Retail adds through an unsigned halfword view of the output. */
        *read_sectors = *(u16*)read_sectors + ++index;
    }
    while ((offset = --index * sizeof(psyq_cd_ring_record_t)), index >= 0) {
        psyq_cd_ring_record_t* record;
        /* The separate byte induction preserves the original branch-slot shift. */
        record = (psyq_cd_ring_record_t*)((u8*)g_psyq_cd_stream_ring + offset);
        if (record->status == StFREE) {
            /* The stored count is read as an unsigned halfword. */
            *free_sectors = *(u16*)free_sectors + 1;
        }
    }
}
