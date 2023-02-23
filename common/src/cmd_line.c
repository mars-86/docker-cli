#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int exec(const char *cmd)
{
#ifdef __DEBUG
    printf("%s\n", cmd);
#endif
    fflush(stdout);
    return system(cmd);
}

const char *parse_cmdl(const char *base, const char **argv)
{
    char *cmd;
    size_t bytes = strlen(base);

    cmd = (char *)malloc(bytes * sizeof(char));
    if (cmd == NULL)
        return NULL;

    memcpy(cmd, base, bytes);
    while (*argv != NULL) {
        size_t arglen = strlen(*argv), nsize = bytes + arglen + 1;
        cmd = (char *)realloc(cmd, nsize * sizeof(char));
        cmd[bytes] = ' ';
        memcpy(&cmd[bytes + 1], *argv++, arglen);
        bytes = nsize;
    }
    cmd = (char *)realloc(cmd, (bytes + 1) * sizeof(char));
    cmd[bytes] = '\0';

#ifdef __DEBUG
    printf("%s\n", cmd);
#endif
    return cmd;
}

void free_cmdl(void *cmd)
{
    free(cmd);
}
