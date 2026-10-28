/* SCUS_942.21 0x8002004c..0x800200d3. */
#include "psx/libcd.h"

/* Write the CD audio mix (left to left, left to right, right to right, right to left) to the drive's volume
 * registers and apply it. */
s32 CD_vol(CdlATV* volume) {
    psyq_cd_apply_volume(volume);
    return 0;
}
