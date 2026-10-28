/* LIBGPU 80023da8-80023dbc: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of an 8x8 rectangle (TILE_8); the caller fills the other fields. */
void SetTile8(TILE_8* primitive) {
    setTile8(primitive);
}
