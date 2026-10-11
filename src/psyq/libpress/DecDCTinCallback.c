#include "psx/libetc.h"
#include "psx/libpress.h"
#include "psx/types.h"

void DecDCTinCallback(psyq_interrupt_callback_t callback) {
    DMACallback(0, callback);
}
