#include "fft/wldcore.h"

/* Transform a render position into the coordinate space selected by mode.
 *
 * The callee receives the adjacent position and dimensions registers as one
 * eight-byte aggregate. Dimensions are preserved by the ABI but unused here.
 */
void wldcore_window_transform_point_for_mode(s32 mode, wldcore_window_render_bounds16_t bounds, s32* output) {
    switch (mode) {
    case 0:
    case 1:
        output[0] = bounds.position.x / 4 + 0x180 + mode * 64;
        output[1] = bounds.position.y;
        break;
    case 2:
        output[0] = (bounds.position.x / 4) + 0x240;
        output[1] = bounds.position.y + 0x100;
        break;
    }
}
