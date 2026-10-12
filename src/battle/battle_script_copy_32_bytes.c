#include "fft/battle.h"
#include "psx/types.h"

/* Copy one 32-byte matrix (battle_copy_bytes); the map light thread copies its base colour matrix
 * with it. */
void battle_script_copy_32_bytes(void* destination, const void* source) {
    battle_copy_bytes(destination, source, 0x20);
}
