#include "psx/libpress.h"

/* Wait until the MDEC has taken its input (MDEC_in_sync). No code refers to it. */
void DecDCTinSync(void) {
    MDEC_in_sync();
}
