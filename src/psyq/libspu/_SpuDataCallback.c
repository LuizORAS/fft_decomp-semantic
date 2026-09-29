/* SCUS_942.21 0x80019404..0x80019427. */
#include "psx/libspu.h"

#include "psx/libetc.h"

void _SpuDataCallback(void (*callback)(void)) {
    DMACallback(4, callback);
}
