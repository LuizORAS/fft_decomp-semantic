#include "fft/main.h"
#include "fft/menu.h"

/* Free the high overlay reservation made by menu_reserve_high_overlay. WORLD compiles this text
 * too. */
void menu_free_high_overlay(void) {
    main_heap_free(g_main_heap_high_overlay_load_address);
}
