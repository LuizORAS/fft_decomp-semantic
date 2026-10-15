#include "psx/libcd.h"

/* Return the name of CD command `command` for debug output, or the unknown name past the 28 commands.
 * Nothing in the game calls it. */
char* CdComstr(u8 command) {
    return command < 28 ? g_psyq_cd_command_names[command] : g_psyq_cd_unknown_name;
}
