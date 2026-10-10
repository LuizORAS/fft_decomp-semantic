#include "fft/battle.h"
#include "psx/types.h"

/* Knockback: copy the destination tile that the attacker's record (src) holds for
 * its target into the target's (dst) movement destination and start the target's
 * one-step knockback path. */
void battle_move_transfer_target_coordinates(battle_unit_misc_data_t* src, battle_unit_misc_data_t* dst) {
    dst->movement.bytes.destination_x = src->target_new_x;
    dst->movement.bytes.destination_y = src->target_new_y;
    dst->movement.bytes.destination_z = src->target_new_map_level;
    battle_move_init_knockback(dst);
}
