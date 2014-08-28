#ifndef __INC_RC4_H
#define __INC_RC4_H

typedef struct {
    unsigned char S[256];
} RC4, *PRC4;

int rc4_init(PRC4 pContext, char *pszKey, int iKeyLen);
int rc4_cipher(PRC4 pContext, char *pszData, int iDataLen);


#endif /* __INC_RC4_H */
