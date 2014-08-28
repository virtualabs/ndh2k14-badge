#include "rc4.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Initialize the RC4 structure with a given key.
 */

int rc4_init(PRC4 pContext, char *pszKey, int iKeyLen)
{
    unsigned char j,t;
    int i;

    if ((iKeyLen == 0) || (pszKey == NULL))
        return 1;

    /* Identity. */
    for (i=0; i<256; i++)
        pContext->S[i] = i;

    /* Shuffle based on key. */
    j=0;
    for (i=0; i<256; i++)
    {
        /* Compute j. */
        j = (j + pContext->S[i] + pszKey[i % iKeyLen])%256;

        /* Swap S[i] and S[j]. */
        t = pContext->S[i];
        pContext->S[i] = pContext->S[j];
        pContext->S[j] = t;
    }

    /* Success. */
    return 0;
}


/**
 * Encrypt/Decrypt data based on the current context.
 */

int rc4_cipher(PRC4 pContext, char *pszData, int iDataLen)
{
    unsigned char i=0,j=0,t,z;
    int k;
    
    if ((pszData == NULL) || (iDataLen == 0))
        return 1;

    for (k=0; k<iDataLen; k++)
    {
        /* Compute next key byte from keystream. */
        i = (i+1)%256;
        j = (j + pContext->S[i])%256;
        t = pContext->S[i];
        pContext->S[i] = pContext->S[j];
        pContext->S[j] = t;

        /* Xor byte with key byte. */
        z = pContext->S[(pContext->S[i] + pContext->S[j])%256];
        pszData[k] = (pszData[k] ^ z);
    }

    return 0;
}
