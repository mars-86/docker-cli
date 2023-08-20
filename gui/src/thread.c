#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "../inc/thread.h"

void *daemon_start(void *args)
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
    int status = pthread_create(tid, NULL, daemon_start, daemon_path);

    if (status > 0)
        return DOCKERCLIE_CANNOTINITDAE;

    return DOCKERCLIE_OK;
}

void *start_daemon_healthcheck(void *args)
{
    daemon_res_t dres;
    while (1) {
        check_daemon_status();
        parse_daemon_response(&dres, DAEMON_RES_STATUS);
#ifdef __DEBUG
        printf("gui: %s %s %s\n", dres.proto, dres.code, dres.code_text);
        printf("res: %d\n", atoi(dres.code));
#endif
        if (atoi(dres.status->t_code) != 200)
        /* sleep 30 sec */
        Sleep(30000);
    }

    return NULL;
}

DOCKERCLI_CODE daemon_healthcheck(const char *path, char *args, pthread_t *tid)
{
    char daemon_path[MAX_PATH];

    sprintf(daemon_path, "%s%s %s", path, "\\dockerd.exe", args ? args : "");
#ifdef __DEBUG
    printf("%s\n", daemon_path);
#endif
    int status = pthread_create(tid, NULL, start_daemon_healthcheck, daemon_path);

    if (status > 0)
        return DOCKERCLIE_CANNOTINITDAE;

    return DOCKERCLIE_OK;
}
