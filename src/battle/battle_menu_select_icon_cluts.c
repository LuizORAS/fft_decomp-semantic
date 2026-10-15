#include "fft/battle.h"

/* Choose the CLUTs of the four menu-frame sprites: the dimmed set while the
 * previous thread is running or g_battle_menu_input_lock_mode is 1. */
void battle_menu_select_icon_cluts(world_menu_icon_record_t* frame) {
    if (battle_thread_is_previous_running() != 0 || g_battle_menu_input_lock_mode == 1) {
        frame->base.sprites[0].clut = 0x7d3c;
        frame->base.sprites[1].clut = 0x7dfc;
        frame->base.sprites[2].clut = 0x7e3c;
        frame->base.sprites[3].clut = 0x7c7c;
    } else {
        frame->base.sprites[0].clut = 0x7c3c;
        frame->base.sprites[1].clut = 0x7d7c;
        frame->base.sprites[2].clut = 0x7dbc;
        frame->base.sprites[3].clut = 0x7cbc;
    }
}
