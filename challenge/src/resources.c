#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "resources.h"

int rsrc_find(int id, char *buffer, int len)
{
    int i;

    for (i=0; i<NB_RSRC; i++)
    {
        if ((gRsrc[i].id == id) && (gRsrc[i].len <= len))
        {
            memcpy(buffer, gRsrc[i].raw, gRsrc[i].len);
            return gRsrc[i].len;
        }
    }

    return -1;
}
