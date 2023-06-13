#include <stdio.h>
#include <string.h>
#include "../../common/common.h"
#include "../inc/process.h"
#include "../../constants/inc/error_codes.h"

/*
int init_daemon(const char *path, char *args, PROCESS_INFORMATION *proc)
{
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;
    char daemon_path[MAX_PATH];

    memset(&sinfo, 0, sizeof(sinfo));
    memset(&pinfo, 0, sizeof(pinfo));

    sinfo.cb = sizeof(sinfo);
    // sinfo.dwFlags = STARTF_USESHOWWINDOW;
    sinfo.wShowWindow = SW_HIDE;

    sprintf(daemon_path, "%s\\dockerd.exe", path);
    int status = CreateProcessA(
        daemon_path,
        args,
        NULL,
        NULL,
        0,
        NORMAL_PRIORITY_CLASS | CREATE_NEW_PROCESS_GROUP | CREATE_SUSPENDED,
        NULL,
        NULL,
        &sinfo,
        &pinfo
    );

    if (!status) {
        win_system_error2("Create Process");
        return DOCKERCLIE_SYSTEM;
    }

    *proc = pinfo;
    // WaitForSingleObject(pinfo.hProcess, INFINITE);

    // CloseHandle(pinfo.hProcess);
    // CloseHandle(pinfo.hThread);

    return DOCKERCLIE_OK;
}
*/

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
