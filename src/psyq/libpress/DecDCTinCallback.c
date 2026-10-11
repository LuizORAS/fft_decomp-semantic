#include "psx/libetc.h"
#include "psx/libpress.h"
#include "psx/types.h"

s32 DecDCTinCallback(psyq_interrupt_callback_t callback) {
    /* Psy-Q returns the previous callback address through a long result. */
    return (s32)DMACallback(0, callback);
}
