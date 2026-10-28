#include "fft/main.h"

/* Finish a unit built from an ENTD or party record: its equipment stats, the equipment's attribute
 * bonuses with base Speed, PA and MA stored again, Move and Jump with full HP and MP, and its names. */
void main_unit_apply_equipment_move_jump_and_name(battle_stats_t* unit) {
    main_unit_set_equipment_stats(unit);
    main_unit_set_equipment_attributes(unit, 1);
    main_unit_calculate_move_jump(unit, 0);
    main_unit_store_character_names(unit);
}
