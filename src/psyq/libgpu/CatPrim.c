/* LIBGPU 80023c2c-80023c50. */
#include "psx/libgpu.h"

void CatPrim(void* primitive, void* next) {
    ((P_TAG*)primitive)->addr = (u32)next;
}
