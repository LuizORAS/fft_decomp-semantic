/* SCUS_942.21 0x80020760..0x8002083f. */
#include "psx/libcd.h"

void callback(void) {
    psyq_cd_dispatch_callbacks();
}
