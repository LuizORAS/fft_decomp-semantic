#include "fft/battle.h"

/* Start a secondary effect on a unit as both its caster and its target, with the
 * unit's sprite palette as the effect's parameter. A unit without battle data
 * starts nothing.
 *
 * The init record is a stack local of which battle_effect_build_secondary_init
 * sets five fields; battle_effect_init_secondary copies the rest of its caster
 * and target blocks and its used_weapon_id as the stack left them. */
void battle_effect_set_secondary(battle_unit_misc_data_t* unit, s32 effect_type) {
    battle_effect_secondary_init_t data;

    battle_effect_build_secondary_init(unit, &data);
    if (unit->battle_data != 0) {
        battle_effect_init_secondary(effect_type, unit->battle_data->sprite_palette, &data);
    }
}
