#include "fft/world.h"

/* Same as main_system_reset_game: the soft reset that WORLD's pad read
 * (world_input_read_tutorial_or_controller) and the tutorial step run when the reset buttons are held
 * (MAIN_INPUT_RESET_BUTTONS). */
void world_input_reset_game(void) {
    main_system_reset_game();
}
