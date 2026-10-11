#include "psx/libetc.h"
#include "psx/libpress.h"
#include "psx/types.h"

s32 DecDCToutCallback(psyq_interrupt_callback_t callback) {
    /* Psy-Q returns the previous callback address through a long result. */
    return (s32)DMACallback(1, callback);
}
