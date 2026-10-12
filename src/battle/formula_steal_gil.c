#include "fft/battle_action.h"

/* Formula 0x27: steal (Level * SP) gil, hit (SP+X)%; physical evade. */
void formula_steal_gil(void) {
    if (formula_calculate_physical_evade() == 0) {
        formula_store_speed_and_x();
        if (formula_calculate_physical_status_accuracy() == 0) {
            formula_calculate_stolen_gil();
        }
    }
}
