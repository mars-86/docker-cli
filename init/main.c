#include <stdio.h>
#include <windows.h>
#include <pthread.h>
#include "../common/common.h"

void *start_docker(void *args)
{
    const char *cmd = (const char *)args;
    exec(cmd);

    return NULL;
}

int main(int argc, char *argv[])
{
    char docker_cli_path[MAX_PATH];

    if (initialize_shared() < 0)
        return EXIT_FAILURE;

    sprintf(docker_cli_path, "%s\\docker-cli.exe", "..\\..\\gui\\bin");

    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&tid, &attr, start_docker, docker_cli_path);

    // fix
    Sleep(2000);
    pthread_attr_destroy(&attr);

    return DOCKERCLIE_OK;
}
