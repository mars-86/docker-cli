#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/common.h"

int main(int argc, char *argv[])
{
    if (argc == 1) {
        system("wsl -d docker-cli -- docker --help");
        return 1;
    }
    const char *cmd = parse_cmdl("wsl -d docker-cli -- docker", (const char **)(argv + 1));

#ifdef __DEBUG
    printf("%s\n", cmd);
#endif
    system(cmd);
    free(cmd);

    return 0;
}
