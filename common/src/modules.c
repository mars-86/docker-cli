#include <stdio.h>
#include "../inc/modules.h"
#include "../inc/process.h"

DOCKERCLI_CODE check_daemon_status(void)
{
    int status = exec("wsl -d docker-cli curl -i -s --unix-socket /var/run/docker.sock -X GET http://localhost/containers/json > C:\\Users\\spa_1\\docker-cli\\tmp\\dockerd-status");
    
    if (status < 0)
        return DOCKERCLIE_CHECKDAESTAT;

    return DOCKERCLIE_OK;
}

DOCKERCLI_CODE parse_daemon_response(daemon_res_t *res)
{
    FILE *fres;
    char buff[512], *buffp;

    if (!(fres = fopen("C:\\Users\\spa_1\\docker-cli\\tmp\\dockerd-status", "r")))
        return DOCKERCLIE_SYSTEM;

    int c;
    /* get proto and code */
    while ((c = fgetc(fres)) != '\n')
        *buffp++ = c;
    *buffp = '\0';

    
    return DOCKERCLIE_OK;
}