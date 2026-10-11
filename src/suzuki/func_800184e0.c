#include "psx/suzuki.h"
#include "psx/types.h"

/* Stores a sound-driver word (D_80032A28) that the driver only ever writes. No
 * function in the tree calls it. */
void func_800184e0(u16 value) {
    D_80032A28 = value;
}
