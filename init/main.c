#include <stdio.h>
#include <string.h>
#include <windows.h>

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

    memset(&sinfo, 0, sizeof(sinfo));
    memset(&pinfo, 0, sizeof(pinfo));

    sinfo.cb = sizeof(sinfo);

    int status = CreateProcessA(
        "C:\\Users\\spa_1\\docker-cli\\daemon\\dockerd.exe",
        NULL,
        NULL,
        NULL,
        0,
        NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &sinfo,
        &pinfo
    );

    if (!status) {
        perror_win("Create Process");
        return -1;
    }

    WaitForSingleObject(pinfo.hProcess, INFINITE);

    CloseHandle(pinfo.hProcess);
    CloseHandle(pinfo.hThread);

    return 0;
}
