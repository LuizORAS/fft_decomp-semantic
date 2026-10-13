#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* Clear bit 0 of script variable EVENT_SCRIPT_VAR_CAMERA_ZOOM (entry 6 of the camera variable
 * indices). The camera threads call it after each step. */
void world_camera_clear_zoom_variable_low_bit(void) {
    s32* word = &g_script_variables[g_world_camera_script_variable_indices[EVENT_CAMERA_COMPONENT_ZOOM]];
    *word &= ~1;
}
