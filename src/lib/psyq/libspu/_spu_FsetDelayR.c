/* SCUS_942.21 0x8001934c..0x80019377. */
#include "psx/libspu.h"

void _spu_FsetDelayR(void) {
    *g_psyq_spu_delay_register = (*g_psyq_spu_delay_register & 0xf0ffffff) | 0x20000000;
}
