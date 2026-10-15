/* LIBGPU 80023fd4-8002400c. */
#include "psx/libgpu.h"

/* Merge `second` into `first`'s packet when both fit in 32 words with one tag: `first` takes the joined length
 * and `second`'s tag word becomes 0. Returns 0, or -1 when they do not fit. No code refers to it. */
int MargePrim(void* first, void* second) {
    int length = ((P_TAG*)first)->len + ((P_TAG*)second)->len + 1;
    int result;

    if (length <= 32) {
        ((P_TAG*)first)->len = length;
        *(u32*)second = 0;
        result = 0;
    } else {
        result = -1;
    }
    return result;
}
