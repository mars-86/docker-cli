#include <stdio.h>
#include "daemon.h"
#include <windows.h>

int main(int argc, char *argv[])
{
    pthread_t tid;
    printf("Initializing docker engine...\n");
    init_dockerd(&tid);
    /* fix */
    Sleep(2000);

    return 0;
}
