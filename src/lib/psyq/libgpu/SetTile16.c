/* LIBGPU 80023dbc-80023dd0: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a 16x16 rectangle (TILE_16); the caller fills the other fields. */
void SetTile16(TILE_16* primitive) {
    setTile16(primitive);
}
