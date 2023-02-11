#include <stdio.h>
#include "../inc/init.h"

static void *startup(void *args)
{
    system("wsl -d docker-cli -- dockerd");
    return NULL;
}

void show_banner(void)
{
    printf(
        "______                                     \n"
       "/ __   \\____  _____ __ __ ____ ______     __\n"
      "/ /  \\  / __ \\/ ___// /  /  __ /  __ / __/  /\n"
     "/ /___/ / /_/ / /___/    / ____/  ___// __  / \n"
    "/_______/\\____/\\____/_/\\__\\___//__/ _\\ \\____/  \n"
    );
}

void init_dockerd(pthread_t *tid)
{
    pthread_create(tid, NULL, startup, NULL);
    pthread_detach(*tid);
}
