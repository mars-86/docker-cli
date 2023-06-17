#include <stdio.h>
#include <windows.h>
#include "daemon.h"

#define BASE_ARGS "-d docker-cli -- start-dockerd"

int main(int argc, char *argv[])
{
    show_banner();
    fputs("\nInitializing docker engine... ", stdout);

    const char *args = parse_cmdl(BASE_ARGS, (const char **)(argv + 1));

    pthread_t daemon_tid;
    int status;
    status = init_daemon(WSL_PATH, (char *)args, &daemon_tid);

    free_cmdl((char *)args);

    if (status != DOCKERCLIE_OK) {
        docker_cli_error(status);
        return status;
    }
    puts("OK");

    int daemon_terminate;
    /* sleep is not a robust way to wait for events, it should be done */
    /* using a condition variable and mutex but windows doesn't support  */
    /* PTHREAD_PROCESS_SHARED attribute */
    while (!(daemon_terminate = get_condition_var(DOCKER_CLI_DAEMON))) {
#ifdef __DEBUG
        puts("Waiting for event...");
        printf("%d\n", daemon_terminate);
#endif
        Sleep(2000);
    }

    pthread_cancel(daemon_tid);
    pthread_join(daemon_tid, NULL);

    fputs("Shutting down dockerd... ", stdout);
    status = exec("wsl -t docker-cli");
    puts("OK");

    fputs("Dockerd gracefully stopped\n", stdout);

    return 0;
}
