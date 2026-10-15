/* SCUS_942.21 0x80019378..0x800193a3. */
#include "psx/libspu.h"

/* Set the SPU's memory delay register for a DMA read: bits 24-27 set to 2 and bit 29 set. _spu_t and
 * _spu_Fr_ call it before reading. */
void _spu_FsetDelayW(void) {
    *g_psyq_spu_delay_register = (*g_psyq_spu_delay_register & 0xf0ffffff) | 0x22000000;
}
