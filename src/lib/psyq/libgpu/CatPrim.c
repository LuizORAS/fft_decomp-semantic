/* LIBGPU 80023c2c-80023c50. */
#include "psx/libgpu.h"

/* Link `next` after `primitive`. No code refers to it. */
void CatPrim(void* primitive, void* next) {
    catPrim(primitive, next);
}
