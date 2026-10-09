#include "fft/battle.h"
#include "fft/main.h"

int main_overlay_call_battle_return_zero(void) {
    /* Script variable 0xff gates the BATTLE call. */
    if (g_main_script_variables[0xff] == 0) {
        return 0;
    }
    return battle_return_zero();
}
