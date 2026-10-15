/* SCUS_942.21 0x8001e894..0x8001e8c3. */
#include "psx/libcd.h"

/* Give the stream its ring of `sectors` sector records (and their data after them) and empty it
 * (StClearRing). OPEN's movie player calls it. */
void StSetRing(void* buffer, s32 sectors) {
    g_psyq_cd_stream_ring = buffer;
    g_psyq_cd_stream_ring_sectors = sectors;
    StClearRing();
}
