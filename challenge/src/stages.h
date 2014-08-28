#ifndef __INC_STAGES_H
#define __INC_STAGES_H

#include "serial.h"
#include "rc4.h"

/**
 * Stage 3 PRNG context
 */

typedef struct {
    char bytes[8];
} PRNG, *PPRNG;

static const char CHARSET[62] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

/**
 * Stages utility functions
 */


int caesar(char *data, uchar size);

/**
 * Stages prototypes
 */

void stage1(char *code);
void stage2(char *code);
void stage3(char *code);

#endif /* __INC_STAGES_H */
