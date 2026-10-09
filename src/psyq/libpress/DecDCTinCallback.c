#include "psx/libetc.h"
#include "psx/libpress.h"
#include "psx/types.h"

void DecDCTinCallback(void* callback) {
    DMACallback(0, callback);
}
