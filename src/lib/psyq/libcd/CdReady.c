#include "psx/libcd.h"

/* Same as CdSync for the data-ready interrupt, through CD_ready. */
int CdReady(int mode, u8* result) {
    return CD_ready(mode, result);
}
