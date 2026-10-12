#include "fft/world.h"
#include "psx/types.h"

/* Return a menu buffer's pages to the arena and mark its allocation slot free. */
void world_menu_free_memory(void* buffer_pointer) {
    u32 buffer = (u32)buffer_pointer;
    u32 grid_base;
    s32* allocation_size;
    menu_allocation_t* allocation;
    s32* allocation_end;
    u8* allocation_grid;
    s32 invalid_buffer;
    s32 page;

    if (buffer < (u32)g_main_heap_companion_overlay_load_address && buffer != -1) {
        invalid_buffer = -1;
        grid_base = (buffer - (u32)g_world_menu_buffer_arena) >> 8;
        allocation_grid = g_world_menu_buffer_allocation_map;
        /* A separate page-count cursor preserves the native count-field base. */
        allocation_size = &g_world_menu_buffer_allocations[0].page_count;
        allocation = g_world_menu_buffer_allocations;
        allocation_end = &g_world_menu_buffer_allocations[16].page_count;
        for (;;) {
            if ((u32)allocation->buffer == buffer) {
                allocation->buffer = (u8*)invalid_buffer;
                /* Written as (index + base) + grid so the two addu operands
                 * come out in the target's order; the size is reloaded
                 * each iteration because the byte store may alias it. */
                for (page = 0; page < *allocation_size; page++) {
                    *(u8*)((page + grid_base) + (u32)allocation_grid) = 0;
                }
                return;
            }
            allocation_size += sizeof(menu_allocation_t) / sizeof(*allocation_size);
            allocation++;
            if ((s32)allocation_size >= (s32)allocation_end) {
                world_thread_exit_current();
                return;
            }
        }
    }
}
