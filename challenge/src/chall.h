#ifndef __INC_CHALL_H
#define __INC_CHALL_H

/* device compatibility: */
#ifndef GICR    /* ATMega*8 don't have GICR, use MCUCR instead */
#   define GICR     MCUCR
#endif

/* Request constants used by USBasp */
#define SKULL_FUNC_READ         1
#define SKULL_FUNC_WRITE        2

#ifndef ulong
#   define ulong    unsigned long
#endif
#ifndef uint
#   define uint     unsigned int
#endif
#ifndef uchar
#   define uchar    unsigned char
#endif

#define BADGE_SERIAL_READ       0x60
#define BADGE_SERIAL_WRITE      0x61
#define BADGE_SERIAL_CHECK      0x62
#define BADGE_SERIAL_PENDING    0x63


#endif /* __INC_CHALL_H */
