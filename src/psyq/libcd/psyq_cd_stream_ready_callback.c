#include "psx/libcd.h"
void psyq_cd_stream_ready_callback(u8 unused_event, u8* unused_result) {
    StCdInterrupt();
}
