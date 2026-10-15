/* SCUS_942.21 0x8001934c..0x80019377. */
#include "psx/libspu.h"

/* Set the SPU's memory delay register for a DMA write: bits 24-27 cleared and bit 29 set. _spu_t calls it
 * before writing. */
void _spu_FsetDelayR(void) {
    *g_psyq_spu_delay_register = (*g_psyq_spu_delay_register & 0xf0ffffff) | 0x20000000;
}
