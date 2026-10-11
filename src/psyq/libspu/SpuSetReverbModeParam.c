/* SCUS_942.21 0x8001a014..0x8001a523. */
#include "psx/libspu.h"

static inline void psyq_spu_copy_bytes(u8* destination, u8* source, s32 size) {
    s32 i;
    for (i = size - 1; i != -1; i--) {
        *destination++ = *source++;
    }
}

s32 SpuSetReverbModeParam(SpuReverbAttr* attr) {
    psyq_spu_reverb_parameters_t parameters;
    s32 clear_work = 0;
    s32 was_enabled = 0;
    s32 mode_changed = 0;
    s32 delay_changed = 0;
    s32 feedback_changed = 0;
    u32 mask = attr->mask;
    s32 all = mask == 0;
    s32 mode;
    s32 delay;
    s32 delay_a;
    s32 delay_b;
    s32 feedback;
    u16 control;
    parameters.mask = 0;
    if (all || (mask & 1)) {
        mode = attr->mode;
        if (mode & 0x100) {
            mode &= ~0x100;
            clear_work = 1;
        }
        if ((u32)mode >= 10 || _SpuIsInAllocateArea_(_spu_rev_startaddr[mode])) {
            return -1;
        }
        mode_changed = 1;
        _spu_rev_attr.mode = mode;
        _spu_rev_offsetaddr = _spu_rev_startaddr[_spu_rev_attr.mode];
        psyq_spu_copy_bytes((u8*)&parameters, (u8*)&_spu_rev_param[_spu_rev_attr.mode], sizeof(parameters));
        switch (_spu_rev_attr.mode) {
        case 7:
            _spu_rev_attr.feedback = 127;
            _spu_rev_attr.delay = 127;
            break;
        case 8:
            _spu_rev_attr.feedback = 0;
            _spu_rev_attr.delay = 127;
            break;
        default:
            _spu_rev_attr.feedback = 0;
            _spu_rev_attr.delay = 0;
            break;
        }
    }
    if (all || (mask & 8)) {
        switch (_spu_rev_attr.mode) {
        case 7:
        case 8:
            delay_changed = 1;
            if (!mode_changed) {
                psyq_spu_copy_bytes((u8*)&parameters, (u8*)&_spu_rev_param[_spu_rev_attr.mode], sizeof(parameters));
                parameters.mask = 0x0c011c00;
            }
            delay = attr->delay;
            _spu_rev_attr.delay = delay;
            delay_a = (delay << 13) / 127;
            delay_b = (delay << 12) / 127;
            parameters.values[10] = delay_a - parameters.values[0];
            parameters.values[11] = delay_b - parameters.values[1];
            parameters.values[12] = delay_b + parameters.values[13];
            parameters.values[16] = delay_b + parameters.values[17];
            parameters.values[26] = delay_b + parameters.values[28];
            parameters.values[27] = delay_b + parameters.values[29];
            break;
        default:
            _spu_rev_attr.delay = 0;
            break;
        }
    }
    if (all || (mask & 0x10)) {
        switch (_spu_rev_attr.mode) {
        case 7:
        case 8:
            feedback_changed = 1;
            if (!mode_changed) {
                if (!delay_changed) {
                    psyq_spu_copy_bytes((u8*)&parameters, (u8*)&_spu_rev_param[_spu_rev_attr.mode], sizeof(parameters));
                    parameters.mask = 0x80;
                } else {
                    parameters.mask |= 0x80;
                }
            }
            feedback = attr->feedback;
            _spu_rev_attr.feedback = feedback;
            parameters.values[7] = ((feedback * 129) << 8) / 127;
            break;
        default:
            _spu_rev_attr.feedback = 0;
            break;
        }
    }
    if (mode_changed) {
        was_enabled = (_spu_RXX->rxx.control >> 7) & 1;
        if (was_enabled) {
            control = _spu_RXX->rxx.control;
            control &= 0xff7f;
            _spu_RXX->rxx.control = control;
        }
    }
    if (!mode_changed) {
        if (all || (mask & 2)) {
            _spu_RXX->rxx.reverb_depth_left = attr->depth.left;
            _spu_rev_attr.depth.left = attr->depth.left;
        }
        if (all || (mask & 4)) {
            _spu_RXX->rxx.reverb_depth_right = attr->depth.right;
            _spu_rev_attr.depth.right = attr->depth.right;
        }
    } else {
        _spu_RXX->rxx.reverb_depth_left = 0;
        _spu_RXX->rxx.reverb_depth_right = 0;
        _spu_rev_attr.depth.left = 0;
        _spu_rev_attr.depth.right = 0;
    }
    if (mode_changed || delay_changed || feedback_changed) {
        _spu_setReverbAttr(&parameters);
    }
    if (clear_work) {
        SpuClearReverbWorkArea(_spu_rev_attr.mode);
    }
    if (mode_changed) {
        _spu_FsetRXX(PSYQ_SPU_REG_REVERB_START, _spu_rev_offsetaddr, PSYQ_SPU_REGISTER_RAW);
        if (was_enabled) {
            control = _spu_RXX->rxx.control;
            control |= 0x80;
            _spu_RXX->rxx.control = control;
        }
    }
    return 0;
}
