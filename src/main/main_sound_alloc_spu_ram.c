#include "psx/libspu.h"
#include "psx/types.h"

/* Reserve `size` bytes of SPU RAM (SpuMalloc) and return their address: the waveset upload
 * (main_sound_load_waveset) is the only caller. */
s32 main_sound_alloc_spu_ram(s32 size) {
    return SpuMalloc(size);
}
