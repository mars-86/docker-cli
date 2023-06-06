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

int init_daemon(const char *path, char *args)
{
    char daemon_path[MAX_PATH];

    sprintf(daemon_path, "%s%s %s", path, "\\daemon\\dockerd.exe", args ? args : "");
    int status = exec(daemon_path);

    if (status < 0)
        return ECANNOTINITDAE;

    return EOK;
}
