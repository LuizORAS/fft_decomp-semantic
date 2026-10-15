#include "fft/world.h"
#include "psx/types.h"

/* Free WORLD's load work buffer (g_world_load_work_buffer), which world_card_init_menu_state allocates;
 * world_card_run_menu_screen calls it when the save screen closes. */
void world_card_free_load_work_buffer(void) {
    main_heap_free(g_world_load_work_buffer);
}
