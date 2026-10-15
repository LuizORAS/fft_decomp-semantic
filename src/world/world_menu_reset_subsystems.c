#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_reset_subsystems. */
void world_menu_reset_subsystems(void) {
    world_script_seed_random_from_vsync();
    world_menu_clear_window_buffer_pointers();
    world_gfx_clear_tpage7_vram_allocation_grid();
    world_menu_init_buffer_allocator();
    world_thread_reset_scheduler();
}
