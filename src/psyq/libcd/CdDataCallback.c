#include "psx/libcd.h"
#include "psx/libetc.h"

/* Set the routine called when a CD-ROM DMA transfer ends (DMACallback on channel 3) and return the
 * previous one. */
void* CdDataCallback(void* callback) {
    return DMACallback(PSYQ_CD_DMA_CHANNEL, callback);
}
