#include "psx/libcd.h"
void psyq_cd_stream_ready_callback(void) {
    StCdInterrupt();
}
