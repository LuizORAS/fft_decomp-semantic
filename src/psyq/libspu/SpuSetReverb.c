/* SCUS_942.21 0x80019e38..0x80019f07. */
#include "psx/libspu.h"

s32 SpuSetReverb(s32 on_off) {
    /* An unpinned pointer exchanges the retail v0/v1 register roles. */
    register volatile psyq_spu_registers_t* registers __asm__("$2");
    u16 control;
    /* The alias preserves the separate case-0 store and MMIO ordering. */
    extern s32 state __asm__("_spu_rev_flag");
    switch (on_off) {
    case 0:
        registers = _spu_RXX;
        control = registers->control;
        state = 0;
        control &= 0xff7f;
        break;
    case 1:
        if (_spu_rev_reserve_wa != on_off && _SpuIsInAllocateArea_(_spu_rev_offsetaddr)) {
            registers = _spu_RXX;
            control = registers->control;
            _spu_rev_flag = 0;
            control &= 0xff7f;
        } else {
            registers = _spu_RXX;
            control = registers->control;
            _spu_rev_flag = on_off;
            control |= 0x80;
        }
        break;
    default:
        goto finished;
    }
    registers->control = control;
finished:
    return _spu_rev_flag;
}
