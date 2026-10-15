#include "fft/battle.h"

/* Formula 0x4E, 25 abilities: Cloud's limits (Braver, Cross-slash, Meteorain, Omnislash, Cherry
 * Blossom), the Ice, Fire and Thunder Bracelets, and monster attacks such as Choco Meteor, Flame
 * Attack, Giga Flare and Midgar Swarm: the magical evade check, then XA = MA and YA = Y as magical
 * damage without Faith (battle_formula_calculate_magical_damage_without_faith). */
void battle_formula_damage_ma_times_y(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        battle_formula_store_ma_and_y();
        battle_formula_calculate_magical_damage_without_faith();
    }
}
