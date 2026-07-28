#ifndef NETX90_DEFS_H_
#define NETX90_DEFS_H_

#include <stdint.h>

#define SET_BIT(REG, BIT) ((REG) |= (BIT))

#define SET_BIT_POS(REG, BIT) ((REG) |= (1 << BIT))

#define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))

#define CLEAR_BIT_POS(REG, BIT) ((REG) &= ~(1 << BIT))

#define READ_BIT(REG, BIT) ((REG) & (BIT))

#define CLEAR_REG(REG) ((REG) = (0x0))

#define WRITE_REG(REG, VAL) ((REG) = (VAL))

#define READ_REG(REG) ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK) WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL) (__CLZ(__RBIT(VAL)))

#if !defined(UNUSED)
#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#endif

// https://stackoverflow.com/a/3553321
#define MEMBER_SIZE(__TYPE__, __MEMBER__) sizeof(((__TYPE__ *)0)->__MEMBER__)

#define ARRAY_COUNT(__ARRAY__) (sizeof((__ARRAY__)) / sizeof((__ARRAY__)[0]))

#define decrementCounter(__COUNTER__) \
    do {                              \
        if ((__COUNTER__) > 0) {        \
            (__COUNTER__)--;            \
        }                             \
    } while (0)

#define incrementCounter(__COUNTER__) \
    do {                              \
        if ((__COUNTER__ + 1) > __COUNTER__) {        \
            (__COUNTER__)++;            \
        }                             \
    } while (0)

#define decrementCounter_Rollover(__COUNTER__, __MINVALUE__, __MAXVALUE__) \
    do {                                                                   \
        if ((__COUNTER__) == (__MINVALUE__)) {                                 \
            (__COUNTER__) = (__MAXVALUE__);                                    \
        } else {                                                           \
            (__COUNTER__)--;                                                   \
        }                                                                  \
    } while (0)

#define incrementCounter_Rollover(__COUNTER__, __MINVALUE__, __MAXVALUE__) \
    do {                                                                   \
        if ((__COUNTER__) < (__MAXVALUE__)) {                                  \
            (__COUNTER__)++;                                                 \
        } else {                                                           \
            (__COUNTER__) = (__MINVALUE__);                                      \
        }                                                                  \
    } while (0)

#define decrementIndex_Rollover(__COUNTER__, __MINVALUE__, __ENUMSIZE__) \
    do {                                                                \
        if ((__COUNTER__) == (__MINVALUE__)) {                              \
            (__COUNTER__) = ((__ENUMSIZE__) - 1);                           \
        } else {                                                        \
            (__COUNTER__)--;                                                \
        }                                                               \
    } while (0)

#define incrementIndex_Rollover(__COUNTER__, __MINVALUE__, __ENUMSIZE__) \
    do {                                                                \
        if ((__COUNTER__) < ((__ENUMSIZE__) - 1)) {                         \
            (__COUNTER__)++;                                              \
        } else {                                                        \
            (__COUNTER__) = (__MINVALUE__);                                   \
        }                                                               \
    } while (0)

uint32_t u8ArrayToU32(uint8_t array[], uint8_t len);
uint16_t u8ArrayToU16(uint8_t array[], uint8_t len);
int32_t u8ArrayTo32(uint8_t array[], uint8_t len);
int16_t u8ArrayTo16(uint8_t array[], uint8_t len);

#endif /* NETX90_DEFS_H_ */

