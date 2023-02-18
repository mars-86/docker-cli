#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/common.h"

#define BASE_CMD "wsl -d docker-cli -- docker"
#define HELP_CMD "wsl -d docker-cli -- docker --help"

int main(int argc, char *argv[])
{
    if (argc == 1) {
        exec(HELP_CMD);
        return 1;
    }
    const char *cmd = parse_cmdl(BASE_CMD, (const char **)(argv + 1));

    exec(cmd);
    free_cmdl(cmd);

    return 0;
}
