#include "fft/battle.h"
#include "fft/battle_script.h"

void script_copy_matrix(MATRIX* destination, const MATRIX* source) {
    battle_copy_bytes(destination, source, sizeof(*destination));
}
