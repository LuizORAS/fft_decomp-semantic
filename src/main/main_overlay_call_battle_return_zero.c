#include "fft/battle.h"
#include "fft/main.h"

/* Return battle_return_zero() while BATTLE.BIN is loaded, else 0. No C caller. */
int main_overlay_call_battle_return_zero(void) {
    /* Script variable 0xff gates the BATTLE call. */
    if (g_main_script_variables[0xff] == 0) {
        return 0;
    }
    return battle_return_zero();
}
