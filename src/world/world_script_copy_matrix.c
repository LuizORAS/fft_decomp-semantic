#include "fft/world.h"
#include "psx/types.h"

void world_script_copy_matrix(MATRIX* destination, const MATRIX* source) {
    world_script_copy_bytes(destination, source, sizeof(*destination));
}
