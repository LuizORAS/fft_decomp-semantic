#include "fft/battle.h"
#include "psx/types.h"

/* Keep the raw button word in g_battle_script_unfiltered_controller_input, then
 * apply the script's disabled and forced inputs to it in place. */
void battle_script_filter_controller_input(u32* input) {
    g_battle_script_unfiltered_controller_input = *input;
    battle_script_apply_input_overrides(input);
}
