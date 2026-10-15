#include "psx/libcd.h"

/* Wait until the CD-ROM DMA has finished (mode 0), or only check (mode 1), through CD_datasync. */
int CdDataSync(int mode) {
    return CD_datasync(mode);
}
