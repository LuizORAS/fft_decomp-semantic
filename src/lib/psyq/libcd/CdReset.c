/* SCUS_942.21 0x8001ea0c..0x8001ea77. */
#include "psx/libcd.h"

/* Reset the CD-ROM side. Mode 2 only reinstalls the interrupt handler (CD_initintr); any other mode resets
 * the drive (CD_init), and mode 1 also sets the CD audio volumes (CD_initvol). Returns 1, or 0 when a step
 * fails. CdInit and main_file_reset_cd_subsystems call it. */
s32 CdReset(s32 mode) {
    if (mode == 2) {
        CD_initintr();
        return 1;
    }
    if (CD_init())
        return 0;
    if (mode == 1 && CD_initvol())
        return 0;
    return 1;
}
