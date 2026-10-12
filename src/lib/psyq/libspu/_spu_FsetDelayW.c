/* SCUS_942.21 0x80019378..0x800193a3. */
#include "psx/libspu.h"

void _spu_FsetDelayW(void) {
    *g_psyq_spu_delay_register = (*g_psyq_spu_delay_register & 0xf0ffffff) | 0x22000000;
}
