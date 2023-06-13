#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../common/common.h"

#define DOCKER_CLI_HOME_VAR_NAME "DOCKER_CLI_HOME"

int main(int argc, char *argv[])
{
    int status = 0;
    if ((status = exec("..\\..\\installer\\bin\\installer")) > 0) {
        docker_cli_error(status);
        return status;
    }

    /* TODO: refresh env vars */

    // char docker_cli_path[MAX_PATH];
    // const char *docker_cli_home_path = getenv(DOCKER_CLI_HOME_VAR_NAME);
    // sprintf(docker_cli_path, "%s\\bin\\docker-cli", docker_cli_home_path);

    if ((status = exec("..\\..\\init\\bin\\docker-cli-init")) > 0) {
        docker_cli_error(status);
        return status;
    }

    return status;
}
