#include <stdio.h>

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

    return cmd;
}
