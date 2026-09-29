/* LIBGPU 80023bf0-80023c2c. */
#include "psx/libgpu.h"

void AddPrims(u32* ot, void* first, void* last) {
    ((P_TAG*)last)->addr = ((P_TAG*)ot)->addr;
    ((P_TAG*)ot)->addr = (u32)first;
}
