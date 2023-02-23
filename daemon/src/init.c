#include <stdio.h>
#include "../inc/init.h"

static void *startup(void *cmd)
{
    exec(cmd);
    return NULL;
}

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

void init_dockerd(pthread_t *tid, void *cmd)
{
    pthread_create(tid, NULL, startup, cmd);
    pthread_detach(*tid);
}
