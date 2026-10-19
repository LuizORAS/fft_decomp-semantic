/* SCUS_942.21 0x800186c4..0x800186e3. */
#include "psx/libspu.h"

/* Start the SPU from cold (_SpuInit(0)): reset every voice and register. main_boot_run_startup calls it. */
void SpuInit(void) {
    _SpuInit(0);
}
