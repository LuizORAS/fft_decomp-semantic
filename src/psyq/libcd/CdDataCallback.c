#include "psx/libcd.h"
#include "psx/libetc.h"

void* CdDataCallback(void* callback) {
    return DMACallback(PSYQ_CD_DMA_CHANNEL, callback);
}
