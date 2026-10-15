#include "psx/libcd.h"

/* Wait until the last command has completed (mode 0), or only check (mode 1), through CD_sync; the result
 * bytes go to result. MAIN's file loader, OPEN's title and file code and WLDCORE's image stream use it. */
int CdSync(int mode, u8* result) {
    return CD_sync(mode, result);
}
