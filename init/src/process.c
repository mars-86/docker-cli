#include <stdio.h>
#include <string.h>
#include <windows.h>
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

int init_daemon(const char *path, char *args)
{
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;
    char daemon_path[MAX_PATH];

    memset(&sinfo, 0, sizeof(sinfo));
    memset(&pinfo, 0, sizeof(pinfo));

    sinfo.cb = sizeof(sinfo);

    sprintf(daemon_path, "%s\\dockerd.exe", path);
    int status = CreateProcessA(
        daemon_path,
        args,
        NULL,
        NULL,
        0,
        NORMAL_PRIORITY_CLASS | DETACHED_PROCESS,
        NULL,
        NULL,
        &sinfo,
        &pinfo
    );

    if (!status) {
        perror_win2("Create Process");
        return ESYSTEM;
    }

    WaitForSingleObject(pinfo.hProcess, INFINITE);

    CloseHandle(pinfo.hProcess);
    CloseHandle(pinfo.hThread);
}
