#include "psx/libcd.h"
char* CdIntstr(u8 interrupt) {
    return interrupt < 7 ? g_psyq_cd_interrupt_names[interrupt] : g_psyq_cd_unknown_name;
}
