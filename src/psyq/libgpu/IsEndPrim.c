/* LIBGPU 80023b98-80023bb4. */
#include "psx/libgpu.h"

int IsEndPrim(void* primitive) {
    return ((P_TAG*)primitive)->addr == PSYQ_GPU_DMA_END;
}
