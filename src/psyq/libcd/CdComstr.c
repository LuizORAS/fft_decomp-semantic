#include "psx/libcd.h"
char* CdComstr(u8 command) {
    return command < 28 ? g_psyq_cd_command_names[command] : g_psyq_cd_unknown_name;
}
