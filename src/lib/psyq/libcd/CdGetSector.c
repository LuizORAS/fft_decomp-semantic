#include "psx/libcd.h"

/* Copy `words` 32-bit values of the current sector from the drive's buffer to destination by DMA (CD_getsector);
 * returns 1. */
int CdGetSector(void* destination, int words) {
    return CD_getsector(destination, words) == 0;
}
