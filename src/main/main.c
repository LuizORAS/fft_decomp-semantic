#include "fft/main.h"
#include "psx/types.h"

/* Program entry: boot, save the base stack pointer for main_system_reset_game, then run the
 * game loop, which never returns (the PadStop/StopCallback tail is unreachable). */
void main(void) {
    main_boot_run_startup();
    main_system_store_stack_pointer(&g_main_system_game_loop_stack_pointer);
    main_system_run_game_loop();
    PadStop();
    StopCallback();
}
