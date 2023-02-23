#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <windows.h>
#include "../inc/finish.h"

void clean_up(void)
{

}

static void *start_daemon(void *args)
{
    FILE *pipe;
    if ((pipe = _popen(".\\daemon\\dockerd", "r")) == NULL)
        printf("Error opening pipe\n");

    int ret = _pclose(pipe);
    // exec(".\\daemon\\dockerd");
    return NULL;
}

void init_daemon(void)
{
    pthread_t tid;
    pthread_create(&tid, NULL, start_daemon, NULL);
    pthread_detach(tid);
    Sleep(2);
}
