/* SCUS_942.21 0x80019e38..0x80019f07. */
#include "psx/libspu.h"

s32 SpuSetReverb(s32 on_off) {
    u16 control;
    switch (on_off) {
    case 0:
        _spu_rev_flag = 0;
        control = _spu_RXX->rxx.control;
        control &= 0xff7f;
        _spu_RXX->rxx.control = control;
        break;
    case 1:
        if (_spu_rev_reserve_wa != on_off && _SpuIsInAllocateArea_(_spu_rev_offsetaddr)) {
            _spu_rev_flag = 0;
            control = _spu_RXX->rxx.control;
            control &= 0xff7f;
            _spu_RXX->rxx.control = control;
        } else {
            _spu_rev_flag = on_off;
            control = _spu_RXX->rxx.control;
            control |= 0x80;
            _spu_RXX->rxx.control = control;
        }
        break;
    }
    return _spu_rev_flag;
}
