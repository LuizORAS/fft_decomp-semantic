#include "psx/libspu.h"
#include "psx/suzuki.h"

/* Reserve `size` bytes of SPU RAM (SpuMalloc) and return their address: the waveset upload
 * (suzuki_sound_load_waveset) is the only caller. */
s32 suzuki_sound_alloc_spu_ram(s32 size) {
    return SpuMalloc(size);
}
