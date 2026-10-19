/* SCUS_942.21 0x800202a8..0x800202f7. */
#include "psx/libcd.h"
#include "psx/libetc.h"

/* Clear the callbacks and the drive status, reset the interrupt callbacks and install the library's
 * interrupt handler (callback) on interrupt 2, the CD-ROM. */
void CD_initintr(void) {
    CD_cbready = 0;
    CD_cbsync = 0;
    CD_status1 = 0;
    CD_status = 0;
    /* Retail resets all four status bytes with a 32-bit store. */
    ResetCallback();
    InterruptCallback(2, callback);
}
