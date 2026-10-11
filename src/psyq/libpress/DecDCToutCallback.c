#include "psx/libetc.h"
#include "psx/libpress.h"
#include "psx/types.h"

void DecDCToutCallback(void* callback) {
    DMACallback(1, callback);
}
