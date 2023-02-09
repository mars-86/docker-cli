#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *parse_cmdl(const char **argv)
{
    const char *base = "wsl -d docker-cli -- docker";
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

int main(int argc, char *argv[])
{
    if (argc == 1) {
        system("wsl -d docker-cli -- docker --help");
        return 1;
    }
    char *cmd = parse_cmdl((const char **)(argv + 1));

#ifdef __DEBUG
    printf("%s\n", cmd);
#endif
    system(cmd);
    free(cmd);

    return 0;
}
