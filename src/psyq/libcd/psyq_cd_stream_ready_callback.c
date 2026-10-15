#include "psx/libcd.h"

/* Ready callback of a streaming read: StCdInterrupt. */
void psyq_cd_stream_ready_callback(void) {
    StCdInterrupt();
}
