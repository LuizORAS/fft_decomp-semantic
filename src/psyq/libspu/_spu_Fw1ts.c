/* SCUS_942.21 0x800193a4..0x80019403. */
#include "psx/libspu.h"

/* A short busy delay (240 multiplications) that lets the SPU settle between register writes. */
void _spu_Fw1ts(void) {
    volatile s32 count;
    volatile s32 value;
    value = 13;
    for (count = 0; count < 240; count++) {
        value *= 3;
    }
}
