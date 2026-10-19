#include "fft/memory.h"
#include "fft/script.h"

/* Copy one 32-byte matrix (mem_copy_bytes_nonempty); the map light thread copies its base colour matrix
 * with it. BATTLE and WORLD compile this text. */
void script_copy_matrix(MATRIX* destination, const MATRIX* source) {
    mem_copy_bytes_nonempty(destination, source, sizeof(*destination));
}
