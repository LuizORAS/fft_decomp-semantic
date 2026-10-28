#include "fft/menu.h"
#include "fft/world.h"

/*
 * Populate the Move and Jump bonuses granted by a movement ability.
 *
 * IDs 0x1E6-0x1E8 grant Move +1/+2/+3; 0x1E9-0x1EB grant Jump +1/+2/+3.
 * Other abilities leave both bonuses at 0.
 */
void world_ability_get_move_and_jump_increase_values(s16 ability, world_item_stat_detail_t* out) {
    battle_menu_unit_status_record_t summary;

    menu_clear_stat_preview(&summary, out);
    if (ability >= ABILITY_ID_MOVEMENT_MOVE_PLUS_1 && ability <= ABILITY_ID_MOVEMENT_MOVE_PLUS_3) {
        out->state.move = ability - (ABILITY_ID_MOVEMENT_MOVE_PLUS_1 - 1);
    } else {
        out->state.move = 0;
    }
    if (ability >= ABILITY_ID_MOVEMENT_JUMP_PLUS_1 && ability <= ABILITY_ID_MOVEMENT_JUMP_PLUS_3) {
        out->state.jump = ability - (ABILITY_ID_MOVEMENT_JUMP_PLUS_1 - 1);
    } else {
        out->state.jump = 0;
    }
}
