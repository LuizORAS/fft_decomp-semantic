#include "fft/world.h"
#include "psx/types.h"

void world_menu_display_text_string(s32 image, s32 origin, s32 text) {
    g_world_thread_call_target = (void (*)(void))world_menu_display_text;
    world_thread_call_on_main_stack(0, image, origin, text);
}
