/* LIBGPU 80023d94-80023da8: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a one-pixel rectangle (TILE_1); the caller fills the other
 * fields. */
void SetTile1(TILE_1* primitive) {
    setTile1(primitive);
}
