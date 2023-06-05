#include <stdio.h>
#include <windows.h>
#include "init.h"

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

    sprintf(docker_cli_path, "%s\\docker-cli.exe", ".\\gui\\bin");
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
        perror_win("Create Process");
        return ESYSTEM;
    }

    ResumeThread(pinfo.hThread);
    // WaitForSingleObject(pinfo.hProcess, INFINITE);

    // CloseHandle(pinfo.hProcess);
    // CloseHandle(pinfo.hThread);}

    return EOK;
}
