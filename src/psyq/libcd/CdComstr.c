#include "psx/libcd.h"
char* CdComstr(u8 command) {
    return command < 28 ? CD_comstr[command] : g_psyq_cd_unknown_name;
}
