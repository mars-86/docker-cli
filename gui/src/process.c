#include <stdio.h>
#include <string.h>
#include "../../common/common.h"
#include "../inc/process.h"
#include "../../constants/inc/error_codes.h"

void perror_win2(const char *msg)
{
        WCHAR *buff;
        FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buff, 0, NULL);
        fprintf(stderr, "%s: %S\n", msg, buff);
        LocalFree(buff);
}

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
        perror_win2("Create Process");
        return ESYSTEM;
    }

    *proc = pinfo;
    // WaitForSingleObject(pinfo.hProcess, INFINITE);

    // CloseHandle(pinfo.hProcess);
    // CloseHandle(pinfo.hThread);

    return EOK;
}
*/

void *start_daemon(void *args)
{
    char *cmd = (char *)args;

    exec("C:\\Users\\spa_1\\docker-cli\\daemon\\dockerd.exe");
    return NULL;
}

int init_daemon(const char *path, char *args, pthread_t *tid)
{
    char daemon_path[MAX_PATH];

    sprintf(daemon_path, "%s %s %s", path, "\\dockerd.exe", args ? args : "");
    int status = pthread_create(tid, NULL, start_daemon, daemon_path);

    if (status > 0)
        return ECANNOTINITDAE;

    return EOK;
}

int check_daemon_status(void)
{
    int status = exec("wsl -d docker-cli curl -i -s --unix-socket /var/run/docker.sock -X GET http://localhost/containers/json");
    printf("daemon check: %d\n", status);

    return EOK;
}
