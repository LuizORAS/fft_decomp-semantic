#include "psx/types.h"

/* WORLD twin of battle_menu_set_text_origin. */
void world_menu_set_text_origin(s32 x, s32 y) {
    *(s16*)0x80153284 = x;
    *(s16*)0x80153286 = y;
}
