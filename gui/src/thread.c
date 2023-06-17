#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "../inc/thread.h"

#define REQUEST_CONTAINERS "curl -i -s --unix-socket /var/run/docker.sock -X GET http://localhost/containers/json"

void *start_daemon(void *args)
{
    const char *cmd = (const char *)args;

    int status = exec(cmd);
    return NULL;
}

DOCKERCLI_CODE init_daemon(const char *path, char *args, pthread_t *tid)
{
    char daemon_path[MAX_PATH];

    sprintf(daemon_path, "%s%s %s", path, "\\dockerd.exe", args ? args : "");
#ifdef __DEBUG
    printf("%s\n", daemon_path);
#endif
    int status = pthread_create(tid, NULL, start_daemon, daemon_path);

    if (status > 0)
        return DOCKERCLIE_CANNOTINITDAE;

    return DOCKERCLIE_OK;
}

DOCKERCLI_CODE check_daemon_status(void)
{
    char daemon_stat_path[MAX_PATH], status_cmd[512];

    sprintf(daemon_stat_path, "%s%s", getenv("DOCKER_CLI_HOME"), "\\tmp\\dockerd-status");
    sprintf(status_cmd, "%s %s %s > %s", WSL_PATH, WSL_DOCKER, REQUEST_CONTAINERS, daemon_stat_path);
    int status = exec(status_cmd);

    if (status < 0)
        return DOCKERCLIE_CHECKDAESTAT;

    return DOCKERCLIE_OK;
}
