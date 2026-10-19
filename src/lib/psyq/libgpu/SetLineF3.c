/* LIBGPU 80023e0c-80023e2c. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a flat-shaded two-segment polyline (LINE_F3) and the end marker
 * after its last point; the caller fills the other fields. */
void SetLineF3(LINE_F3* primitive) {
    setLineF3(primitive);
}
