#include <stdio.h>
#include "../inc/init.h"
#include "../../common/common.h"
#include "../../constants/inc/error_codes.h"

void show_banner(void)
{
    printf(
        "    ______                                     \n"
        "   / __   \\____  _____ __ __ ____ ______     __\n"
        "  / /  \\  / __ \\/ ___// /  /  __ /  __ / __/  /\n"
        " / /___/ / /_/ / /___/    / ____/  ___// __  / \n"
        "/_______/\\____/\\____/_/\\__\\___//__/ _\\ \\____/  \n"
        "                                               \n"
    );
}

int init_daemon(const char *path, char *args, PROCESS_INFORMATION *proc)
{
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;
    char daemon_path[MAX_PATH];

    memset(&sinfo, 0, sizeof(sinfo));
    memset(&pinfo, 0, sizeof(pinfo));

    sinfo.cb = sizeof(sinfo);
    sinfo.dwFlags = STARTF_USESHOWWINDOW;
    sinfo.wShowWindow = SW_HIDE;

    int status = CreateProcessA(
        path,
        (char *)args,
        NULL,
        NULL,
        0,
        NORMAL_PRIORITY_CLASS | CREATE_NEW_PROCESS_GROUP | CREATE_SUSPENDED | CREATE_NO_WINDOW,
        NULL,
        NULL,
        &sinfo,
        &pinfo
    );

    if (!status)
        return ESYSTEM;

    *proc = pinfo;

    return EOK;
}
