#include "fft/event_equip.h"
#include "fft/menu.h"

void equip_input_clear_state(void) {
    g_menu_input_newly_pressed = 0;
    g_menu_input_secondary_repeat = 0;
    g_menu_input_primary_repeat = 0;
}
