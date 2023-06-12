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
    char daemon_cmd[256];

    sprintf(daemon_cmd, "%s %s %s", path, "-d docker-cli -- start-dockerd", args ? args : "");
    int status = exec(daemon_cmd);

    if (status < 0)
        return DOCKERCLIE_CANNOTINITDAE;

    return DOCKERCLIE_OK;
}
