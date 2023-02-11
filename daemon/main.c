#include <stdio.h>
#include "daemon.h"
#include <windows.h>

int main(int argc, char *argv[])
{
    show_banner();
    printf("\nInitializing docker engine...\n");
    /* fix */
    Sleep(2000);

    pthread_t tid;
    init_dockerd(&tid);
    /* fix */
    Sleep(2000);

    return 0;
}
