#include "psx/libetc.h"
#include "psx/libpress.h"

/* Reset the MDEC (MDEC_reset(mode)); mode 0 first makes sure LIBETC's interrupt handling has started
 * (ResetCallback) and has MDEC_reset send the tables again. open_movie_init_stream calls it with 0. */
void DecDCTReset(s32 mode) {
    if (mode == 0) {
        ResetCallback();
    }
    MDEC_reset(mode);
}
