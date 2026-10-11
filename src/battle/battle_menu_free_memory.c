#include "fft/battle.h"

/* Return a buffer of battle_menu_alloc_memory to the menu arena: its slot record
 * is marked free (-1) and its 256-byte pages are cleared in the occupancy map.
 *
 * A pointer at or above the companion overlay load address, or -1, is ignored. A
 * buffer that none of the 16 slot records holds stops the current thread. */
void battle_menu_free_memory(void* buffer_pointer) {
    u32 buffer = (u32)buffer_pointer;
    u32 grid_base;
    s32* allocation_size;
    u8** allocation_buffer;
    s32* allocation_end;
    u8* allocation_grid;
    s32 invalid_buffer;
    s32 page;

    if (buffer < (u32)g_main_heap_companion_overlay_load_address && buffer != -1) {
        invalid_buffer = -1;
        grid_base = (buffer - (u32)g_battle_menu_buffer_arena) >> 8;
        allocation_grid = g_battle_menu_buffer_allocation_map;
        allocation_size = &g_battle_menu_allocator_slot_records[0].page_count;
        allocation_buffer = &g_battle_menu_allocator_slot_records[0].buffer;
        allocation_end = &g_battle_menu_allocator_slot_records[16].page_count;
        for (;;) {
            if ((u32)*allocation_buffer == buffer) {
                *allocation_buffer = (u8*)invalid_buffer;
                /* Written as (index + base) + grid so the two addu operands
                 * come out in the target's order; the size is reloaded
                 * each iteration because the byte store may alias it. */
                for (page = 0; page < *allocation_size; page++) {
                    *(u8*)((page + grid_base) + (u32)allocation_grid) = 0;
                }
                return;
            }
            allocation_size += sizeof(battle_menu_allocation_t) / sizeof(*allocation_size);
            allocation_buffer += sizeof(battle_menu_allocation_t) / sizeof(*allocation_buffer);
            if ((s32)allocation_size >= (s32)allocation_end) {
                battle_thread_exit_current();
                return;
            }
        }
    }
}
