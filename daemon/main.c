#include <stdio.h>
#include <windows.h>
#include <signal.h>
#include "daemon.h"
#include "../common/common.h"

#define WSL_PATH "C:\\Windows\\System32\\wsl.exe"
#define BASE_ARGS " -d docker-cli -- dockerd"

void perror_win(const char *msg)
{
        WCHAR *buff;
        FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buff, 0, NULL);
        fprintf(stderr, "%s: %S\n", msg, buff);
        LocalFree(buff);
}

static int terminate = 0;

void on_sigint(int sig)
{
    terminate = 1;
}

int main(int argc, char *argv[])
{
    show_banner();
    printf("\nInitializing docker engine...\n");
    /* fix */
    Sleep(1000);

    const char *args = parse_cmdl(BASE_ARGS, (const char **)(argv + 1));

    int status;
    // PROCESS_INFORMATION pinfo;
    // status = init_daemon(WSL_PATH, (char *)args, &pinfo);
    pthread_t tid;
    status = init_daemon(WSL_PATH, (char *)args);

    printf("%d\n", tid);

    if (status) {
        perror_win("Init daemon");
        return status;
    }

    // ResumeThread(pinfo.hThread);

    free_cmdl((char *)args);

    /* set signal handler */
    signal(SIGINT, on_sigint);

    /* fix: wait for sigint */
    while (!terminate)
        Sleep(1000);

    printf("Shuting down dockerd: ");
    status = exec("wsl -t docker-cli");
    Sleep(1000);

    pthread_join(tid, NULL);
    // CloseHandle(pinfo.hProcess);

    printf("Dockerd gracefully stopped\n");

    return 0;
}
