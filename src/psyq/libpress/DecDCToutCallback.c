#include "psx/libetc.h"
#include "psx/libpress.h"
#include "psx/types.h"

void DecDCToutCallback(psyq_interrupt_callback_t callback) {
    DMACallback(1, callback);
}
