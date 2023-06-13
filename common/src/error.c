#include <stdio.h>
#include <windows.h>
#include "../inc/error.h"

static const char *error_msg[] = {
    "No error",
    "Installation canceled",
    "Docker cli already installed",
    "Error creating pipe",
    "Error installing file system",
    "Error creating folder",
    "Error installing docker",
    "Error initializing daemon",
    "Error checking daemon status"
};

void docker_cli_error(DOCKERCLI_CODE code)
{
    fprintf(stderr, "%s\n", error_msg[code]);
}

void win_system_error(const char *msg)
{
        WCHAR *buff;
        FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buff, 0, NULL);
        fprintf(stderr, "%s: %S\n", msg ? msg : "", buff);
        LocalFree(buff);
}
