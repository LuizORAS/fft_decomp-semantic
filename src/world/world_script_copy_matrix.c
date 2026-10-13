#include "fft/memory.h"
#include "fft/world.h"

void world_script_copy_matrix(MATRIX* destination, const MATRIX* source) {
    copy_bytes_nonempty(destination, source, sizeof(*destination));
}
