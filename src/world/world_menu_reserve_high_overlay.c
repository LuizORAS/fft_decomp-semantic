#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_reserve_high_overlay. */
void world_menu_reserve_high_overlay(u32 size) {
    while (1) {
        if (main_heap_reserve_at(g_main_heap_high_overlay_load_address, size)
            == g_main_heap_high_overlay_load_address) {
            break;
        }
        world_thread_yield();
        if (g_main_font_print_enabled != 0) {
            FntPrint(g_world_text_wait_for_file_allocation_message);
        }
    }
}
