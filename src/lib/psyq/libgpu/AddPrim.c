/* LIBGPU 80023bb4-80023bf0. */
#include "psx/libgpu.h"

/* Link `primitive` at the head of the ordering-table entry `ot`: it takes over the entry's link and the
 * entry points to it, so of the primitives at one entry the last added is drawn first. About 130 functions sort
 * their primitives this way. */
void AddPrim(void* ot, void* primitive) {
    addPrim(ot, primitive);
}
