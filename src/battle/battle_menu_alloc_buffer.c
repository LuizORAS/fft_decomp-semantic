#include "fft/battle_menu.h"
#include "psx/types.h"

/* Allocate `bytes` from the menu buffer arena (battle_menu_alloc_memory); battle_menu_free_memory
 * returns it. */
void* battle_menu_alloc_buffer(s32 bytes) {
    return battle_menu_alloc_memory(bytes);
}
