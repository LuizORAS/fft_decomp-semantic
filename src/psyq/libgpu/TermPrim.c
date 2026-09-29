/* LIBGPU 80023c50-80023c68. */
#include "psx/libgpu.h"

void TermPrim(void* primitive) {
    ((P_TAG*)primitive)->addr = PSYQ_GPU_DMA_END;
}
