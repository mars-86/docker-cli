#include <stdio.h>
#include <windows.h>
#include "daemon.h"
#include "../common/common.h"

#define BASE_CMD "wsl -d docker-cli -- dockerd"

void perror_win(const char *msg)
{
        WCHAR *buff;
        FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buff, 0, NULL);
        fprintf(stderr, "%s: %S\n", msg, buff);
        LocalFree(buff);
}

int main(int argc, char *argv[])
{
    show_banner();
    printf("\nInitializing docker engine...\n");
    /* fix */
    Sleep(1000);

    const char *cmd = parse_cmdl(BASE_CMD, (const char **)(argv + 1));

    HANDLE jobh = CreateJobObjectA(NULL, "dockerdaemon");

    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli;
    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    SetInformationJobObject(jobh, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));

    if (!jobh) {
        perror_win("Create Job");
        return 99;
    }

    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;
    char daemon_path[MAX_PATH];

    memset(&sinfo, 0, sizeof(sinfo));
    memset(&pinfo, 0, sizeof(pinfo));

    sinfo.cb = sizeof(sinfo);
    sinfo.dwFlags = STARTF_USESHOWWINDOW;
    sinfo.wShowWindow = SW_HIDE;

    int status = CreateProcessA(
        "C:\\Windows\\System32\\wsl.exe",
        " -d docker-cli -- dockerd",
        NULL,
        NULL,
        0,
        NORMAL_PRIORITY_CLASS | CREATE_NEW_PROCESS_GROUP | CREATE_SUSPENDED | CREATE_NO_WINDOW,
        NULL,
        NULL,
        &sinfo,
        &pinfo
    );

    if (!status) {
        perror_win("Create Process");
        return 99;
    }

    AssignProcessToJobObject(jobh, pinfo.hProcess);

    ResumeThread(pinfo.hThread);
    // init_daemon(NULL, NULL);
    // exec(cmd);
    /* fix */
    Sleep(5000);

    free_cmdl((char *)cmd);

    printf("ACA\n");
    system("PAUSE");

    return 0;
}
