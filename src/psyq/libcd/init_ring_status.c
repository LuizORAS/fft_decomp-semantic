/* SCUS_942.21 0x800211c8..0x80021207. */
#include "psx/libcd.h"

void init_ring_status(u32 first_sector, u32 sector_count) {
    u32 sector_index;
    for (sector_index = 0; sector_index < sector_count;) {
        /* The empty barrier preserves the measured loop delay slot. */
        __asm__ volatile("");
        /* Retail clears both halves of the descriptor's status word. */
        *(u32*)&g_psyq_cd_stream_ring[sector_index++ + first_sector].status = 0;
    }
}
