#include "psx/libetc.h"

/* Stop the BIOS's pad handler (StopPAD). main calls it in its unreachable tail. */
void PadStop(void) {
    StopPAD();
}
