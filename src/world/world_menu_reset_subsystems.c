#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Target 0x800f2998. */
void world_menu_reset_subsystems(void) {
    script_seed_random_from_vsync();
    world_menu_clear_window_buffer_pointers();
    world_gfx_clear_tpage7_vram_allocation_grid();
    world_menu_init_buffer_allocator();
    thread_reset_scheduler();
}
