#include "psx/libcd.h"

/* Reset the CD subsystem (CdReset(0)) after a seek or read error; OPEN's XA audio and movie
 * streams and WLDCORE's image stream call it too. */
void main_file_reset_cd_subsystems(void) {
    CdReset(0);
}
