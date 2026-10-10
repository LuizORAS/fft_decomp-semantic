#include "fft/battle.h"
#include "fft/main.h"
#include "psx/types.h"

/* Make the other of BATTLE's two render buffers the current one and clear its
 * 0x180-entry ordering table. */
void main_gfx_swap_and_clear_otag(void) {
    battle_render_buffer_t* battle_data = g_battle_buffer_a;
    u32* otag;

    if (g_battle_gfx_current_render_buffer == battle_data) {
        battle_data++;
    }
    otag = battle_data->otag;
    g_battle_gfx_current_render_buffer = battle_data;
    ClearOTagR(otag, 0x180);
}
