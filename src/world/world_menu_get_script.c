#include "fft/world.h"
#include "psx/types.h"

/* Return one of WORLD's nine resident text blocks (world_text_data_e) by
 * index; index 6 has none. */
u8* world_menu_get_script(world_text_data_e index) {
    return g_world_menu_scripts[index];
}
