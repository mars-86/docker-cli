#include <stdio.h>
#include <string.h>
#include "../../common/common.h"
#include "../inc/thread.h"

void *start_daemon(void *args)
{
    const char *cmd = (const char *)args;

    int status = exec(cmd);
    return NULL;
}

int init_daemon(const char *path, char *args, pthread_t *tid)
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

int check_daemon_status(void)
{
    int status = exec("wsl -d docker-cli curl -i -s --unix-socket /var/run/docker.sock -X GET http://localhost/containers/json &> C:\\Users\\spa_1\\docker-cli\\tmp\\dockerd-status");
    
    if (status < 0)
        return DOCKERCLIE_CHECKDAESTAT;

    return DOCKERCLIE_OK;
}
