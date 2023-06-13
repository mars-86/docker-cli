#include <stdio.h>
#include <windows.h>
#include <pthread.h>
#include "init.h"
#include "../common/common.h"

/*
int main(int argc, char *argv[])
{
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;
    char docker_cli_path[MAX_PATH];

    memset(&sinfo, 0, sizeof(sinfo));
    memset(&pinfo, 0, sizeof(pinfo));

    sinfo.cb = sizeof(sinfo);
    // sinfo.dwFlags = STARTF_USESHOWWINDOW;
    sinfo.wShowWindow = SW_HIDE;

    DWORD flags = NORMAL_PRIORITY_CLASS | CREATE_NEW_PROCESS_GROUP | CREATE_SUSPENDED;

#ifndef __DEBUG
    flags |= CREATE_NO_WINDOW;
#endif

    sprintf(docker_cli_path, "%s\\docker-cli.exe", "..\\..\\gui\\bin");
    int status = CreateProcessA(
        docker_cli_path,
        NULL,
        NULL,
        NULL,
        0,
        flags,
        NULL,
        NULL,
        &sinfo,
        &pinfo
    );

    if (!status) {
        win_system_error("Create Process");
        return DOCKERCLIE_SYSTEM;
    }

    ResumeThread(pinfo.hThread);
    WaitForSingleObject(pinfo.hProcess, INFINITE);

    CloseHandle(pinfo.hProcess);
    CloseHandle(pinfo.hThread);

    return DOCKERCLIE_OK;
}
*/

void *start_docker(void *args)
{
    const char *cmd = (const char *)args;
    exec(cmd);

    return NULL;
}

int main(int argc, char *argv[])
{
    char docker_cli_path[MAX_PATH];

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
