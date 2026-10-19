#include "fft/battle_gfx.h"
#include "psx/types.h"

/* Same as battle_gfx_start_unit_fade_out: the entry of the event instructions that take a unit off the
 * field (battle_script_blue_remove_unit, battle_script_blue_remove_all_enemy_units,
 * battle_script_teleport_out_event_instruction). */
void battle_gfx_start_event_unit_fade_out(s32 runtime_id, s32 amount, s16 red, s16 green, s16 blue) {
    battle_gfx_start_unit_fade_out(runtime_id, amount, red, green, blue);
}
