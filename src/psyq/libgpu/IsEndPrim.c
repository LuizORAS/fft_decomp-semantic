/* LIBGPU 80023b98-80023bb4. */
#include "psx/libgpu.h"

/* Return whether `primitive` ends its chain. No code refers to it. */
int IsEndPrim(void* primitive) {
    return ((P_TAG*)primitive)->addr == PSYQ_GPU_DMA_END;
}
