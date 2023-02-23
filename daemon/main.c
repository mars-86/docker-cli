#include <stdio.h>
#include <windows.h>
#include "daemon.h"
#include "../common/common.h"

#define BASE_CMD "wsl -d docker-cli --"

int main(int argc, char *argv[])
{
    show_banner();
    printf("\nInitializing docker engine...\n");
    /* fix */
    Sleep(2000);

    const char *cmd = parse_cmdl(BASE_CMD, (const char **)argv);

    pthread_t tid;
    init_dockerd(&tid, (char *)cmd);
    /* fix */
    Sleep(5000);

    free_cmdl((char *)cmd);

    return 0;
}
