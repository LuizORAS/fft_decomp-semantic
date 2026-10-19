/* LIBGPU 80023c50-80023c68. */
#include "psx/libgpu.h"

/* End the chain at `primitive`. FntFlush ends its text packets this way. */
void TermPrim(void* primitive) {
    termPrim(primitive);
}
