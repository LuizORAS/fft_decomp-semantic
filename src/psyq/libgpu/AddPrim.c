/* LIBGPU 80023bb4-80023bf0. */
#include "psx/libgpu.h"

void AddPrim(void* ot, void* primitive) {
    ((P_TAG*)primitive)->addr = ((P_TAG*)ot)->addr;
    ((P_TAG*)ot)->addr = (u32)primitive;
}
