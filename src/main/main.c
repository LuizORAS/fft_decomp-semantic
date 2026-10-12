#include "fft/main.h"

/* Program entry: run the power-on startup, save the stack pointer for
 * main_system_reset_game and enter the game loop. The loop never returns, so the
 * pad and callback shutdown below it never runs. */
void main(void) {
    main_boot_run_startup();
    main_system_store_stack_pointer(&g_main_system_game_loop_stack_pointer);
    main_system_run_game_loop();
    PadStop();
    StopCallback();
}
