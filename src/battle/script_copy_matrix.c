#include "fft/battle.h"
#include "fft/battle_script.h"
#include "fft/memory.h"

void script_copy_matrix(MATRIX* destination, const MATRIX* source) {
    mem_copy_bytes_nonempty(destination, source, sizeof(*destination));
}
