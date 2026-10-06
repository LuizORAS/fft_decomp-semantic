#include "fft/battle.h"

/* Formula 0x08, most attack magic (41 abilities: Fire, Bolt and Ice and their levels, Holy, Flare,
 * Meteor, Bio, Melt, Tornado, Quake, Ultima, the summons Shiva to Zodiac): the magical evade check,
 * XA = MA and YA = Y, the element's Strengthen, the magical XA modifiers, XA * YA with the weather
 * and the element, both Faiths, the elemental absorption and, on the 19% roll, the spell's status. */
void battle_formula_magical_damage(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        battle_formula_store_ma_and_y();
        battle_formula_apply_ability_element_strengthen();
        battle_formula_apply_magical_xa_modifiers();
        if (battle_formula_calculate_elemental_xa_times_ya() == 0) {
            battle_formula_calculate_faith();
            if (battle_formula_apply_elemental_absorption_and_status_proc() == 0) {
                battle_formula_apply_status();
            }
        }
    }
}
