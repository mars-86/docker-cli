#include <stdio.h>
#include <stdlib.h>
#include "../inc/process.h"

int exec(const char *cmd)
{
#ifdef __DEBUG
    printf("%s\n", cmd);
#endif
    fflush(stdout);
    return system(cmd);
}
