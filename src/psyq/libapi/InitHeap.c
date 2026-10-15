/* 0x80021f14: BIOS 0xa0:0x39 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's InitHeap (table A, 0x39): give the BIOS's malloc the heap at `arena`, `size` bytes. No code
 * refers to it; the game's memory goes through its own allocators (main_heap_alloc and the others). */
void InitHeap(void* arena, u32 size) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_INIT_HEAP);
}
