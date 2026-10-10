#include "fft/battle.h"

/* Opcodes 2e-77, the rest of the 120-entry handler table: the error report. */
void battle_effect_code_script_2e_invalid(void) {
    main_system_report_stub(1, 0x22);
}
