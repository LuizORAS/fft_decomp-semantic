/* SCUS_942.21 0x80019404..0x80019427. */
#include "psx/libspu.h"

#include "psx/libetc.h"

/* Set the SPU's DMA channel 4 completion callback (DMACallback). SpuStart installs _spu_FiDMA and SpuQuit
 * removes it. */
void _SpuDataCallback(void (*callback)(void)) {
    DMACallback(4, callback);
}
