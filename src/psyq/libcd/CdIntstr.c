#include "psx/libcd.h"
char* CdIntstr(u8 interrupt) {
    return interrupt < 7 ? CD_intstr[interrupt] : g_psyq_cd_unknown_name;
}
