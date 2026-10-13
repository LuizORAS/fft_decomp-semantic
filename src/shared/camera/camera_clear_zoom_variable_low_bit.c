#include "fft/battle_camera.h"
#include "fft/script.h"

/* Clear bit 0 of the zoom variable selected by the camera component-index table. */
void camera_clear_zoom_variable_low_bit(void) {
    g_script_variables[g_camera_script_variable_indices[EVENT_CAMERA_COMPONENT_ZOOM]] &= ~1;
}
