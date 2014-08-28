#include <stdio.h>
#include "hash.h"

void hash(PHASH pHash, char *pBytes, int iSize)
{
    RC4 rc4;
    int i;

    /* Initialize hash structure. */
    rc4_init(&rc4, pBytes, iSize);
    for (i=0; i<8; i++)
        pHash->bytes[i] = pBytes[i%iSize];

    /* Encrypt bytes with itself */
    rc4_cipher(&rc4, (char *)pHash->bytes, 8);
}


int hashcmp(PHASH pHash1, unsigned char *pHash2)
{
    int i;
    
    for (i=0; i<8; i++)
        if (pHash1->bytes[i] != pHash2[i])
            return 0;
    return 1;
}

void hashprint(PHASH pHash, char *buffer)
{
    snprintf(
        buffer,
        17,
        "%02X%02X%02X%02X%02X%02X%02X%02X",
        pHash->bytes[0],
        pHash->bytes[1],
        pHash->bytes[2],
        pHash->bytes[3],
        pHash->bytes[4],
        pHash->bytes[5],
        pHash->bytes[6],
        pHash->bytes[7]
    );
}
