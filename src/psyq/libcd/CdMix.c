#include "psx/libcd.h"

/* Set how the CD audio's left and right channels reach the SPU's left and right inputs (CD_vol); returns 1.
 * main_sound_put_type and OPEN's movie sound call it. */
s32 CdMix(CdlATV* volume) {
    CD_vol(volume);
    return 1;
}
