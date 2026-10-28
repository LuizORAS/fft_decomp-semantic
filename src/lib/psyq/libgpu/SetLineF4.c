/* LIBGPU 80023e4c-80023e6c. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a flat-shaded three-segment polyline (LINE_F4) and the end
 * marker after its last point; the caller fills the other fields. */
void SetLineF4(LINE_F4* primitive) {
    setLineF4(primitive);
}
