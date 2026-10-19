#include "psx/libspu.h"
#include "psx/suzuki.h"

/* Release a block of SPU RAM (SpuFree) and return 0: suzuki_sound_free_waveset frees a waveset's samples
 * this way. */
s32 suzuki_sound_free_spu_ram(u32 addr) {
    SpuFree(addr);
    return 0;
}
