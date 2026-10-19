#include "psx/libcd.h"

/* Ready callback of a streaming read: StCdInterrupt. */
void psyq_cd_stream_ready_callback(u8 unused_event, u8* unused_result) {
    StCdInterrupt();
}
