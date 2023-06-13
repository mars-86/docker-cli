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
    puts("\nInitializing docker engine...");
    /* fix */
    Sleep(1000);

    const char *args = parse_cmdl(BASE_ARGS, (const char **)(argv + 1));

    int status;
    status = init_daemon(WSL_PATH, (char *)args);

    free_cmdl((char *)args);

    if (status != DOCKERCLIE_OK) {
        docker_cli_error(status);
        return status;
    }

    /* set signal handler */
    signal(SIGINT, on_sigint);

    pthread_mutex_lock(&daemon_thread_mutex);

    while (!daemon_terminate)
        pthread_cond_wait(&daemon_thread_cond, &daemon_thread_mutex);

    pthread_mutex_unlock(&daemon_thread_mutex);

    fputs("Shutting down dockerd... ", stdout);
    status = exec("wsl -t docker-cli");
    puts("OK");

    fputs("Dockerd gracefully stopped\n", stdout);

    return 0;
}
