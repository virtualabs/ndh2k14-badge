#ifndef __INC_HASH_H
#define __INC_HASH_H

#include "rc4.h"

/**
 * Own hashing function based on RC4.
 */

typedef struct {
    unsigned char bytes[8];
} HASH, *PHASH;

void hash(PHASH pHash, char *pBytes, int iSize);
int hashcmp(PHASH pHash1, unsigned char *pHash2);
void hashprint(PHASH pHash, char *buffer);

#endif /* __INC_HASH_H */
