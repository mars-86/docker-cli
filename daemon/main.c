#include <stdio.h>
#include <windows.h>
#include <signal.h>
#include "daemon.h"
#include "../common/common.h"

#define WSL_PATH "C:\\Windows\\System32\\wsl.exe"
#define BASE_ARGS " -d docker-cli -- dockerd"

static int terminate = 0;

void on_sigint(int sig)
{
    terminate = 1;
}

int main(int argc, char *argv[])
{
    show_banner();
    printf("\nInitializing docker engine...\n");
    /* fix */
    Sleep(1000);

    const char *args = parse_cmdl(BASE_ARGS, (const char **)(argv + 1));

    int status;
    status = init_daemon(WSL_PATH, (char *)args);

    free_cmdl((char *)args);

    if (status) {
        win_system_error("Init daemon");
        return status;
    }

    /* set signal handler */
    signal(SIGINT, on_sigint);

    /* fix: wait for sigint */
    while (!terminate)
        Sleep(1000);

    printf("Shuting down dockerd: ");
    status = exec("wsl -t docker-cli");
    Sleep(1000);

    printf("Dockerd gracefully stopped\n");

    return 0;
}
