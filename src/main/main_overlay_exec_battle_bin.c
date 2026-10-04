#include "fft/main.h"
#include "psx/types.h"

/* Load BATTLE.BIN (sector 1000) into the whole overlay area and run its entry point. */
void main_overlay_exec_battle_bin(void) {
    main_file_load_to_address_checked(1000,
        g_main_heap_high_overlay_load_address - g_main_heap_low_overlay_load_address,
        g_main_heap_low_overlay_load_address);
    main_overlay_call_battle_entrypoint();
}
