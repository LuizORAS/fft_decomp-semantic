#include "fft/menu.h"
#include "psx/types.h"

/* Return the text id a substitution code stands for: 0xE0 (TEXT_FORMAT_RAMZA_NAME) the first unit name,
 * 0x4000; 0xE1, 0xE5, 0xE9 and 0xEA `value` plus the unit, location, item or ability name base; 0xEB `value`
 * itself; -1 for any other code. Several modules compile this text. */
s32 text_resolve_format_string_id(s32 format_code, s32 value) {
    if (format_code == TEXT_FORMAT_RAMZA_NAME) {
        return TEXT_ID_UNIT_NAME_SPECIAL_BASE;
    }
    if (format_code == TEXT_FORMAT_UNIT_NAME) {
        return value + TEXT_ID_UNIT_NAME_SPECIAL_BASE;
    }
    if (format_code == TEXT_FORMAT_LOCATION_NAME) {
        return value + TEXT_ID_LOCATION_NAME_BASE;
    }
    if (format_code == TEXT_FORMAT_ITEM_NAME) {
        return value + TEXT_ID_ITEM_NAME_BASE;
    }
    if (format_code == TEXT_FORMAT_ABILITY_NAME) {
        return value + TEXT_ID_ABILITY_NAME_BASE;
    }
    if (format_code == TEXT_FORMAT_EXPLICIT_ID) {
        return value;
    }
    return -1;
}
