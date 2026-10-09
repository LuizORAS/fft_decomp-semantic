/* SCUS_942.21 0x8001a014..0x8001a523. */
#include "psx/libspu.h"

s32 SpuSetReverbModeParam(SpuReverbAttr* attr) {
    psyq_spu_reverb_parameters_t parameters;
    /* Volatile retains the retail stack flag and its independent later reload. */
    volatile s32 clear_work = 0;
    s32 was_enabled = 0;
    s32 mode_changed = 0;
    s32 delay_changed = 0;
    s32 feedback_changed = 0;
    /* This view preserves the flag store before the caller-mask load. */
    u32 mask = ((volatile SpuReverbAttr*)attr)->mask;
    s32 all = mask == 0;
    s32 mode;
    s32 count;
    u8* destination;
    u8* source;
    s32 delay;
    s32 delay_a;
    s32 delay_b;
    s32 feedback;
    /* This view preserves the two explicit signed range comparisons. */
    extern s32 current_reverb_mode __asm__("_spu_rev_attr_mode");
    s32 reverb_mode;
    parameters.mask = 0;
    if (all || (mask & 1)) {
        mode = attr->mode;
        if (mode & 0x100) {
            mode &= ~0x100;
            {
                /* The pin retains the retail t0 flag-store value. */
                register s32 value __asm__("$8") = 1;
                clear_work = value;
            }
        }
        if ((u32)mode >= 10 || _SpuIsInAllocateArea_(_spu_rev_startaddr[mode])) {
            return -1;
        }
        mode_changed = 1;
        _spu_rev_attr_mode = mode;
        {
            /* The address pin and tie preserve the retail table-address/load scheduling. */
            s32 current_mode = _spu_rev_attr_mode;
            register u32* start_address __asm__("$4") = &_spu_rev_startaddr[current_mode];
            u32 start;
            source = (u8*)&_spu_rev_param[current_mode];
            start = *start_address;
            _spu_rev_offsetaddr = start;
            destination = (u8*)&parameters;
            for (count = 67; count != -1; count--) {
                *destination++ = *source++;
            }
        }
        switch (_spu_rev_attr_mode) {
        case 7:
            _spu_rev_attr_feedback = 127;
            _spu_rev_attr_delay = 127;
            break;
        case 8:
            _spu_rev_attr_feedback = 0;
            _spu_rev_attr_delay = 127;
            break;
        default:
            _spu_rev_attr_feedback = 0;
            _spu_rev_attr_delay = 0;
            break;
        }
    }
    if (all || (mask & 8)) {
        {
            reverb_mode = current_reverb_mode;
            if (reverb_mode >= 9)
                goto delay_zero;
            {
                s32 outside = reverb_mode < 7;
                if (outside)
                    goto delay_zero;
            }
            {
                delay_changed = 1;
                if (!mode_changed) {
                    {
                        u8* copy_out = (u8*)&parameters;
                        u8* copy_in = (u8*)&_spu_rev_param[_spu_rev_attr_mode];
                        s32 copy_count;
                        for (copy_count = 67; copy_count != -1; copy_count--) {
                            *copy_out++ = *copy_in++;
                        }
                    }
                    parameters.mask = 0x0c011c00;
                }
                delay = attr->delay;
                _spu_rev_attr_delay = delay;
                delay_a = (delay << 13) / 127;
                delay_b = (delay << 12) / 127;
                parameters.values[10] = delay_a - parameters.values[0];
                {

                    parameters.values[11] = delay_b - parameters.values[1];
                    parameters.values[12] = delay_b + parameters.values[13];
                    parameters.values[16] = delay_b + parameters.values[17];
                }
                parameters.values[26] = delay_b + parameters.values[28];
                parameters.values[27] = delay_b + parameters.values[29];
            }
            goto delay_done;
        delay_zero: { _spu_rev_attr_delay = 0; }
        delay_done:;
        }
    }
    if (all || (mask & 0x10)) {
        {
            reverb_mode = current_reverb_mode;
            if (reverb_mode >= 9)
                goto feedback_zero;
            {
                s32 outside = reverb_mode < 7;
                if (outside)
                    goto feedback_zero;
            }
            {
                feedback_changed = 1;
                if (!mode_changed) {
                    if (!delay_changed) {
                        {
                            u8* copy_out = (u8*)&parameters;
                            u8* copy_in = (u8*)&_spu_rev_param[_spu_rev_attr_mode];
                            s32 copy_count;
                            for (copy_count = 67; copy_count != -1; copy_count--) {
                                *copy_out++ = *copy_in++;
                            }
                        }
                        parameters.mask = 0x80;
                    } else {
                        parameters.mask |= 0x80;
                    }
                }
                feedback = attr->feedback;
                _spu_rev_attr_feedback = feedback;
                parameters.values[7] = ((feedback * 129) << 8) / 127;
            }
            goto feedback_done;
        feedback_zero: { _spu_rev_attr_feedback = 0; }
        feedback_done:;
        }
    }
    if (mode_changed) {
        /* The control-value pin preserves the retail v1 read/modify/write register. */
        volatile psyq_spu_registers_t* control_registers = _spu_RXX;
        was_enabled = (control_registers->control >> 7) & 1;
        if (was_enabled) {
            register u16 control __asm__("$3") = control_registers->control;
            control_registers->control = control & 0xff7f;
        }
        goto clear_depth;
    }
    {
        if (all || (mask & 2)) {
            _spu_RXX->reverb_depth_left = *(u16*)&attr->depth.left;
            _spu_rev_attr_depth_left = *(u16*)&attr->depth.left;
        }
        if (all || (mask & 4)) {
            _spu_RXX->reverb_depth_right = *(u16*)&attr->depth.right;
            _spu_rev_attr_depth_right = *(u16*)&attr->depth.right;
        }
    }
    goto depth_done;
clear_depth:
    _spu_RXX->reverb_depth_left = 0;
    _spu_RXX->reverb_depth_right = 0;
    _spu_rev_attr_depth_left = 0;
    _spu_rev_attr_depth_right = 0;
depth_done:
    if (mode_changed || delay_changed || feedback_changed) {
        _spu_setReverbAttr(&parameters);
    }
    {
        /* The pin retains the retail t0 reload of the stack flag. */
        register s32 clear_flag __asm__("$8") = clear_work;
        if (clear_flag) {
            SpuClearReverbWorkArea(_spu_rev_attr_mode);
        }
    }
    if (mode_changed) {
        _spu_FsetRXX(PSYQ_SPU_REG_REVERB_START, _spu_rev_offsetaddr, PSYQ_SPU_REGISTER_RAW);
        if (was_enabled) {
            /* Unpinned control pointer changes the final enable-store register. */
            register volatile psyq_spu_registers_t* control_registers __asm__("$2") = _spu_RXX;
            control_registers->control |= 0x80;
        }
    }
    return 0;
}
