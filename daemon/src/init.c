#include <stdio.h>
#include "../inc/init.h"

void show_banner(void)
{
    printf(
        "    ______                                     \n"
        "   / __   \\____  _____ __ __ ____ ______     __\n"
        "  / /  \\  / __ \\/ ___// /  /  __ /  __ / __/  /\n"
        " / /___/ / /_/ / /___/    / ____/  ___// __  / \n"
        "/_______/\\____/\\____/_/\\__\\___//__/ _\\ \\____/  \n"
        "                                               \n"
    );
}

void *start_daemon(void *args)
{
    const char *cmd = args;
    int status = exec("C:\\Windows\\System32\\wsl.exe -d docker-cli -- start-dockerd");

    return NULL;
}

int init_daemon(const char *path, char *args, pthread_t *tid)
{
    char daemon_cmd[256];

    sprintf(daemon_cmd, "%s %s", path, args ? args : "\0");
#ifdef __DEBUG
    fputs(daemon_cmd, stdout);
#endif
    int status = pthread_create(tid, NULL, start_daemon, daemon_cmd);

    if (status)
        return 1;

    return 0;
}
