#include "psx/libcd.h"
#include "psx/libetc.h"

void* CdDataCallback(void* callback) {
    return (void*)DMACallback(PSYQ_CD_DMA_CHANNEL, (psyq_interrupt_callback_t)callback);
}
