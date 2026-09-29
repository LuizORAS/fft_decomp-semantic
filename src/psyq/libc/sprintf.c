/* LIBC 8002233c-80022b98; switch table 800104a4-80010558. */
#include "psx/libc.h"

int sprintf(char* destination, const char* format, ...) {
    char buffer[512];
    psyq_printf_format_t state;
    char* arguments = (char*)__builtin_next_arg(format);
    const char* digits;
    /* Unpinning changes the variadic-cursor allocation and instruction order. */
    register char* argument __asm__("$3");
    char* segment;
    int length;
    int count;
    unsigned int number;
    int hash;
    int zero;
    /* Each unpinned temporary changes the original instruction allocation. */
    register int value __asm__("$2");
    register u32 flags __asm__("$3");
    const char* next;
    int character;
    int minus;
    int plus;
    int space;
    char* terminator;
    const char* current;
    count = 0;
    character = *format;
    if (!character)
        goto finished;
    minus = '-';
    plus = '+';
    space = ' ';
    do {
        if (character != '%')
            goto literal;
        state = g_psyq_libc_default_format;
        hash = '#';
        zero = '0';
        for (;;) {
            current = format;
            format = current + 1;
            character = current[1];
            if (character == minus) {
                state.flags.bits |= PSYQ_PRINTF_LEFT;
                continue;
            }
            if (character == plus) {
                state.flags.bits |= PSYQ_PRINTF_PLUS;
                continue;
            }
            if (character == space) {
                state.flags.bytes.sign = character;
                continue;
            }
            if (character == hash) {
                state.flags.bits |= PSYQ_PRINTF_ALTERNATE;
                continue;
            }
            if (character == zero) {
                state.flags.bits |= PSYQ_PRINTF_ZERO;
                continue;
            }
            break;
        }
        if (character == '*') {
            argument = arguments;
            arguments = argument + sizeof(u32);
            state.width = *(s32*)argument;
            if (state.width < 0) {
                state.width = -state.width;
                state.flags.bits |= PSYQ_PRINTF_LEFT;
            }
            format = current + 2;
            character = current[2];
        } else {
            while ((unsigned)(character - '0') < 10) {
                value = state.width * 10;
                value -= '0';
                /* Without the tie GCC folds the subtraction into the addition. */
                __asm__("" : "=r"(value) : "0"(value));
                state.width = value + character;
                next = format;
                format = next + 1;
                character = next[1];
            }
        }
        if (character == '.') {
            current = format;
            format = current + 1;
            character = current[1];
            if (character == '*') {
                argument = arguments;
                arguments = argument + sizeof(u32);
                state.precision = *(s32*)argument;
                format = current + 2;
                character = current[2];
            } else {
                while ((unsigned)(character - '0') < 10) {
                    value = state.precision * 10;
                    value -= '0';
                    /* Without the tie GCC folds the subtraction into the addition. */
                    __asm__("" : "=r"(value) : "0"(value));
                    state.precision = value + character;
                    next = format;
                    format = next + 1;
                    character = next[1];
                }
            }
            if (state.precision >= 0)
                state.flags.bits |= PSYQ_PRINTF_PRECISION;
        }
        if (state.flags.bits & PSYQ_PRINTF_LEFT)
            state.flags.bits &= ~PSYQ_PRINTF_ZERO;
        segment = buffer + sizeof(buffer);
    conversion:
        switch (character) {
        case 'h':
            state.flags.bits |= PSYQ_PRINTF_SHORT;
            goto next_conversion;
        case 'l':
            state.flags.bits |= PSYQ_PRINTF_LONG;
            goto next_conversion;
        case 'L':
            state.flags.bits |= PSYQ_PRINTF_CAPITAL_L;
        next_conversion:
            next = format;
            format = next + 1;
            character = next[1];
            goto conversion;
        case 'd':
        case 'i':
            argument = arguments;
            arguments = argument + sizeof(u32);
            number = *(u32*)argument;
            flags = state.flags.bits;
            if (flags & PSYQ_PRINTF_SHORT)
                number = (s16)number;
            if ((s32)number < 0) {
                number = -number;
                state.flags.bytes.sign = minus;
            } else {
                value = flags & PSYQ_PRINTF_PLUS;
                if (value)
                    state.flags.bytes.sign = plus;
            }
            goto decimal;
        case 'u':
            argument = arguments;
            arguments = argument + sizeof(u32);
            number = *(u32*)argument;
            /* Keeps the argument load before the subsequent flags load. */
            __asm__("" : "=r"(number) : "0"(number));
            if (state.flags.bits & PSYQ_PRINTF_SHORT)
                number = (u16)number;
            state.flags.bytes.sign = 0;
        decimal:
            if (!(state.flags.bits & PSYQ_PRINTF_PRECISION)) {
                if (state.flags.bits & PSYQ_PRINTF_ZERO) {
                    state.precision = state.width;
                    if (state.flags.bytes.sign)
                        state.precision--;
                }
                if (state.precision <= 0)
                    state.precision = 1;
            }
            length = 0;
            while (number) {
                *--segment = number % 10 + '0';
                number /= 10;
                length++;
            }
            if (length < state.precision) {
                /* Prevents hoisting the zero literal into the preceding delay slot. */
                __asm__ volatile("" : : : "$3");
                zero = '0';
                do {
                    *--segment = zero;
                    length++;
                } while (length < state.precision);
            }
            if (state.flags.bytes.sign) {
                *--segment = state.flags.bytes.sign;
                length++;
            }
            goto write_segment;
        case 'o':
            argument = arguments;
            arguments = argument + sizeof(u32);
            number = *(u32*)argument;
            flags = state.flags.bits;
            if (flags & PSYQ_PRINTF_SHORT)
                number = (u16)number;
            if (!(flags & PSYQ_PRINTF_PRECISION)) {
                value = flags & PSYQ_PRINTF_ZERO;
                if (value)
                    state.precision = state.width;
                if (state.precision <= 0)
                    state.precision = 1;
            }
            length = 0;
            while (number) {
                *--segment = (number & 7) + '0';
                number >>= 3;
                length++;
            }
            if ((state.flags.bits & PSYQ_PRINTF_ALTERNATE) && length) {
                value = '0';
                zero = *segment;
                /* Retains the second zero literal in the following delay slot. */
                __asm__("" : "=r"(value) : "0"(value));
                if (zero != value) {
                    value = '0';
                    *--segment = value;
                    length++;
                }
            }
            zero = '0';
            while (length < state.precision) {
                *--segment = zero;
                length++;
            }
            goto write_segment;
        case 'p':
            state.precision = 8;
            state.flags.bits |= PSYQ_PRINTF_PRECISION | PSYQ_PRINTF_LONG;
        case 'X':
            digits = g_psyq_libc_uppercase_digits;
            goto hexadecimal;
        case 'x':
            digits = g_psyq_libc_lowercase_digits;
        hexadecimal:
            argument = arguments;
            arguments = argument + sizeof(u32);
            number = *(u32*)argument;
            flags = state.flags.bits;
            if (flags & PSYQ_PRINTF_SHORT)
                number = (u16)number;
            if (!(flags & PSYQ_PRINTF_PRECISION)) {
                if (flags & PSYQ_PRINTF_ZERO) {
                    value = flags & PSYQ_PRINTF_ALTERNATE;
                    state.precision = state.width;
                    if (value)
                        state.precision -= 2;
                }
                if (state.precision <= 0)
                    state.precision = 1;
            }
            length = 0;
            while (number) {
                *--segment = digits[number & 15];
                number >>= 4;
                length++;
            }
            if (length < state.precision) {
                /* Prevents hoisting the zero literal into the preceding delay slot. */
                __asm__ volatile("" : : : "$3");
                zero = '0';
                do {
                    *--segment = zero;
                    length++;
                } while (length < state.precision);
            }
            if (state.flags.bits & PSYQ_PRINTF_ALTERNATE) {
                value = '0';
                *--segment = character;
                *--segment = value;
                length += 2;
            }
            goto write_segment;
        case 'c':
            value = (int)arguments;
            arguments = (char*)value + sizeof(u32);
            *--segment = *(u8*)value;
            length = 1;
            goto write_segment;
        case 's':
            value = (int)arguments;
            arguments = (char*)value + sizeof(u32);
            segment = *(char**)value;
            if (state.flags.bits & PSYQ_PRINTF_ALTERNATE) {
                length = *(u8*)segment++;
                if ((state.flags.bits & PSYQ_PRINTF_PRECISION) && state.precision < length)
                    length = state.precision;
            } else if (!(state.flags.bits & PSYQ_PRINTF_PRECISION)) {
                length = strlen(segment);
            } else {
                terminator = memchr(segment, 0, state.precision);
                length = terminator - segment;
                if (!terminator)
                    length = state.precision;
            }
            goto write_segment;
        case 'n':
            value = (int)arguments;
            /* Address-taking retains the original variadic cursor stack home. */
            *(&arguments) = (char*)value + sizeof(u32);
            segment = *(char**)value;
            value = state.flags.bits & PSYQ_PRINTF_SHORT;
            if (value)
                *(s16*)segment = count;
            else
                *(s32*)segment = count;
            goto advance;
        default:
            if (character != '%')
                goto finished;
            goto literal;
        }
    literal:
        destination[count++] = character;
        goto advance;
    write_segment:
        if (length < state.width && !(state.flags.bits & PSYQ_PRINTF_LEFT)) {
            do {
                destination[count++] = space;
                state.width--;
            } while (length < state.width);
        }
        memmove(destination + count, segment, length);
        count += length;
        while (length < state.width) {
            destination[count++] = space;
            length++;
        }
    advance:
        next = format;
        /* Address-taking retains the original format cursor stack home. */
        *(&format) = next + 1;
        character = next[1];
    } while (character);
finished:
    destination[count] = 0;
    return count;
}
