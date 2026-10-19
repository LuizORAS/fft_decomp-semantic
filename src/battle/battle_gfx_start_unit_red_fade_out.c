#include "fft/battle_gfx.h"
#include "psx/types.h"

/* Same as battle_gfx_start_unit_fade_out with a red flash: amount 4 and a red offset of 0x1f. A
 * poached unit (battle_unit_apply_poach_morbol_transformation) and a picked-up crystal or chest
 * (battle_move_start_next_post_move_event) leave the field this way. */
void battle_gfx_start_unit_red_fade_out(s32 runtime_id) {
    battle_gfx_start_unit_fade_out(runtime_id, 4, 0x1F, 0, 0);
}
