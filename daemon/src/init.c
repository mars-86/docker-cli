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
        "============================================================================\n"
        "                                                                            \n"
        "   =====         =====         ===== ||    //   =====   =====     =====     \n"
        " ||      \\\\   //       \\\\   //       ||   //  ||      ||     || ||      \\  \n"
        " ||       || ||         || ||        ||  //   ||      ||     // ||       || \n"
        " ||       || ||         || ||        ||//     ||===== ||=====   ||       || \n"
        " ||       || ||         || ||        ||\\\\     ||      ||\\\\      ||       || \n"
        " ||      //   \\\\       //   \\\\       ||  \\\\   ||      ||  \\\\    ||      //  \n"
        "   =====         =====         ===== ||   \\\\    ===== ||    \\\\    =====     \n"
        "                                                                            \n"
        "============================================================================\n"
    );
}

void init_dockerd(pthread_t *tid)
{
    pthread_create(tid, NULL, startup, NULL);
    pthread_detach(*tid);
}
