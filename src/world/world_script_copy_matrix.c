#include "fft/memory.h"
#include "fft/script.h"

void world_script_copy_matrix(MATRIX* destination, const MATRIX* source) {
    mem_copy_bytes_nonempty(destination, source, sizeof(*destination));
}
