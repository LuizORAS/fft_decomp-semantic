#include "fft/world.h"

void world_free_work_buffer(void) {
    main_heap_free(g_world_load_work_buffer);
}
