#include "fft/battle.h"
#include "psx/types.h"

void battle_script_copy_matrix(MATRIX* destination, const MATRIX* source) {
    battle_copy_bytes(destination, source, sizeof(*destination));
}
