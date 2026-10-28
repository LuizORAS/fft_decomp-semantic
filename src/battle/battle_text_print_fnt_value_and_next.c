#include "fft/battle_text.h"
#include "psx/types.h"

/* Debug print: FntPrint `value` and then `value` + 1 in decimal. The dialog thread
 * (battle_text_run_dialog_thread) calls it with the window's line count for each centered line. */
void battle_text_print_fnt_value_and_next(s32 value) {
    FntPrint(g_battle_text_decimal_format, value);
    value++;
    FntPrint(g_battle_text_decimal_format, value);
}
