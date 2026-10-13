#include "fft/main.h"
#include "fft/menu.h"

void menu_free_high_overlay(void) {
    main_heap_free(g_main_heap_high_overlay_load_address);
}
