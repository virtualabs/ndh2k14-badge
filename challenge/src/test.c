#include <stdlib.h>
#include <stdio.h>
#include "rc4.h"
#include "resources.h"

int main(int argc, char **argv)
{
    RC4 rc4;
    char buffer[256];
    int len;

    len = rsrc_find(1, buffer, 256);

    rc4_init(&rc4, "HelpPlz!" , 8);
    rc4_cipher(&rc4, buffer, len);
    buffer[len] = '\0';
    printf("Output: %s\n", buffer);

    return 0;
}
