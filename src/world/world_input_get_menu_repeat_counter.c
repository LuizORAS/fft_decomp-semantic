#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

s32 world_input_get_menu_repeat_counter(void) {
    return g_script_event_initial_repeat_counter;
}
