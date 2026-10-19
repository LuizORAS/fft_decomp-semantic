/* SCUS_942.21 0x80020760..0x8002083f. */
#include "psx/libcd.h"

/* The CD-ROM interrupt handler CD_initintr installs: read interrupts (getintr) until none is left,
 * calling the ready callback for a data interrupt and the sync callback for a command interrupt, then restore
 * the drive's register bank. */
void callback(void) {
    psyq_cd_dispatch_callbacks();
}
