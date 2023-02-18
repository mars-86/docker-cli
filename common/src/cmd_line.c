#include <stdio.h>
#include <stdlib.h>

int exec(const char *cmd)
{
#ifdef __DEBUG
    printf("%s\n", cmd);
#endif
    return system(cmd);
}

const char *parse_cmdl(const char *base, const char **argv)
{
    char *__cmd;
    size_t __bytes = strlen(base);

    __cmd = (char *)malloc(__bytes * sizeof(char));
    if (__cmd == NULL)
        return NULL;

    memcpy(__cmd, base, __bytes);
    while (*argv != NULL) {
        size_t arglen = strlen(*argv), nsize = __bytes + arglen + 1;
        __cmd = (char *)realloc(__cmd, nsize * sizeof(char));
        __cmd[__bytes] = ' ';
        memcpy(&__cmd[__bytes + 1], *argv++, arglen);
        __bytes = nsize;
    }
    __cmd = (char *)realloc(__cmd, (__bytes + 1) * sizeof(char));
    __cmd[__bytes] = '\0';

    return __cmd;
}
