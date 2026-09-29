/* SCUS_942.21 0x8001423c..0x80014277. */
#include "psx/suzuki.h"

void main_sound_init_heap(void* arena, u32 size) {
    suzuki_heap_block_t* head = arena;
    size &= ~15;
    g_main_sound_heap_end = (u8*)arena + size;
    head->flags = 0x8000;
    g_main_sound_heap_arena = head;
    g_main_sound_heap_size = size;
    g_main_sound_heap_blocks = head;
    head->_unknown_02 = 0;
    head->_unknown_04 = 0;
    head->end = (u8*)(head + 1);
    head->next = 0;
}
