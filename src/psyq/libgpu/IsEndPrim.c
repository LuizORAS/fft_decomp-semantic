/* LIBGPU 80023b98-80023bb4. */
#include "psx/libgpu.h"

int IsEndPrim(void* primitive) {
    return getaddr(primitive) == PSYQ_GPU_DMA_END;
}
