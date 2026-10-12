#include "fft/event_bunit.h"

menu_instruction_t* bunit_cmd_set_semitrans_handler(menu_instruction_t* data) {
    g_bunit_gfx_semitrans_enabled = data->draw_state.value;
    return menu_next_instruction(data);
}
