#include "fft/battle_camera.h"
#include "fft/script.h"

/* Clear bit 0 of the zoom script variable, the one entry EVENT_CAMERA_COMPONENT_ZOOM of
 * g_camera_script_variable_indices names (EVENT_SCRIPT_VAR_CAMERA_ZOOM). The camera threads call it
 * after each step. */
void camera_clear_zoom_variable_low_bit(void) {
    g_script_variables[g_camera_script_variable_indices[EVENT_CAMERA_COMPONENT_ZOOM]] &= ~1;
}
