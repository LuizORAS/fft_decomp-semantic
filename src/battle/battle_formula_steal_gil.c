#include "fft/battle.h"

/* Formula 0x27, Gil Taking and Shine Lover: the physical evade check and the hit chance Speed + X
 * (battle_formula_calculate_physical_status_accuracy), then Speed * Level gil taken
 * (battle_formula_calculate_stolen_gil). */
void battle_formula_steal_gil(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        battle_formula_store_speed_and_x();
        if (battle_formula_calculate_physical_status_accuracy() == 0) {
            battle_formula_calculate_stolen_gil();
        }
    }
}
