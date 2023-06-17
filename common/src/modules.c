#include <stdio.h>
#include <windows.h>
#include "../inc/modules.h"
#include "../inc/process.h"

#define REQUEST_CONTAINERS "curl -i -s --unix-socket /var/run/docker.sock -X GET http://localhost/containers/json"
/* used constants from common.h */
#define WSL_PATH "C:\\Windows\\System32\\wsl.exe"
#define WSL_DOCKER "-d docker-cli"

DOCKERCLI_CODE check_daemon_status(void)
{
    char daemon_stat_path[MAX_PATH], status_cmd[512];

    sprintf(daemon_stat_path, "%s%s", getenv("DOCKER_CLI_HOME"), "\\tmp\\dockerd-status");
    sprintf(status_cmd, "%s %s %s > %s", WSL_PATH, WSL_DOCKER, REQUEST_CONTAINERS, daemon_stat_path);
    int status = exec(status_cmd);

    if (status < 0)
        return DOCKERCLIE_CHECKDAESTAT;

    return DOCKERCLIE_OK;
}

DOCKERCLI_CODE parse_daemon_response(daemon_res_t *res)
{
    FILE *fres;
    char buff[512], *buffp, daemon_stat_path[MAX_PATH];

    sprintf(daemon_stat_path, "%s%s", getenv("DOCKER_CLI_HOME"), "\\tmp\\dockerd-status");
    if (!(fres = fopen(daemon_stat_path, "r")))
        return DOCKERCLIE_SYSTEM;

    int c;
    /* get proto and code */
    while ((c = fgetc(fres)) != '\n')
        *buffp++ = c;
    *buffp = '\0';


    return DOCKERCLIE_OK;
}