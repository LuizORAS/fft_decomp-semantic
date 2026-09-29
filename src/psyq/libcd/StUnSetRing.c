/* SCUS_942.21 0x80020f3c..0x80020f93. */
#include "psx/libcd.h"

void StUnSetRing(void) {
    EnterCriticalSection();
    CdDataCallback(0);
    CdReadyCallback(0);
    *g_psyq_cd_unset_index_register = 0;
    *g_psyq_cd_unset_request_register = 0;
    ExitCriticalSection();
}
