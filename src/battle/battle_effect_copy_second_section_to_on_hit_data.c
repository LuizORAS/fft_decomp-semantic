#include "fft/battle.h"

void battle_effect_copy_second_section_to_on_hit_data(void) {
    g_battle_effect_gravity = on_hit_effects_data_second_section;
}
