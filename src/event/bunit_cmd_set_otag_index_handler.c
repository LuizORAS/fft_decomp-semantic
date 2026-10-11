#include "fft/event_bunit.h"
#include "psx/types.h"

menu_instruction_t* bunit_cmd_set_otag_index_handler(menu_instruction_t* data) {
    g_bunit_gfx_otag_index = data->draw_state.value;
    return menu_next_instruction(data);
}
