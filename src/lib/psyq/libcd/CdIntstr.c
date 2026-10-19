#include "psx/libcd.h"

/* Same as CdComstr for an interrupt (result) code, of seven. Nothing in the game calls it. */
char* CdIntstr(u8 interrupt) {
    return interrupt < 7 ? CD_intstr[interrupt] : g_psyq_cd_unknown_name;
}
