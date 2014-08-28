#include <stdio.h>
#include <stdlib.h>

#include "hash.h"

int main(int argc, char **argv) {
    HASH h;
    char buffer[17];

    if (argc >= 2) {
        hash(&h, argv[1], strlen(argv[1]));
        hashprint(&h, buffer);
        buffer[16] = '\0';
        printf("hash: %s\n", buffer);
    }
    return 0;
}
