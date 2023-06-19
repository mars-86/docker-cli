#include <stdio.h>
#include <windows.h>
#include "../inc/modules.h"
#include "../inc/process.h"

#define REQUEST_CONTAINERS "curl -i -s --unix-socket /var/run/docker.sock -X GET http://localhost/containers/json"
/* use constants from common.h */
#define WSL_PATH "C:\\Windows\\System32\\wsl.exe"
#define WSL_DOCKER "-d docker-cli"

enum _DAEMON_RES_TYPE {
    NUMBER = 0,
    TEXT
};

typedef enum _DAEMON_RES_TYPE DAEMON_RES_TYPE;

static int daemon_res_len[] = {PROTO_LEN, CODE_LEN, CODE_TEXT_LEN, RES_TEXT_LEN};
static int daemon_res_typ[] = {TEXT, NUMBER, TEXT, TEXT};

DOCKERCLI_CODE check_daemon_status(void)
{
    char daemon_stat_path[MAX_PATH], status_cmd[512];

    sprintf(daemon_stat_path, "%s%s", getenv("DOCKER_CLI_HOME"), "\\tmp\\dockerd-status");
    sprintf(status_cmd, "%s %s %s | head -n 1 > %s", WSL_PATH, WSL_DOCKER, REQUEST_CONTAINERS, daemon_stat_path);
    int status = exec(status_cmd);

    if (status < 0)
        return DOCKERCLIE_CHECKDAESTAT;

    return DOCKERCLIE_OK;
}

/* TODO: parse res text and data */
DOCKERCLI_CODE parse_daemon_response(daemon_res_t *res, int flags)
{
    FILE *fres;
    char buff[512], *buffp = buff, daemon_stat_path[MAX_PATH];

    sprintf(daemon_stat_path, "%s%s", getenv("DOCKER_CLI_HOME"), "\\tmp\\dockerd-status");
    if (!(fres = fopen(daemon_stat_path, "r")))
        return DOCKERCLIE_SYSTEM;

    res->status = NULL;
    res->res_text = NULL;
    res->data = NULL;

    int c;
    /* get proto and code */
    if ((flags & DAEMON_RES_STATUS)) {
        if (!(res->status = (daemon_status_t *)malloc(sizeof(daemon_status_t))))
            return DOCKERCLIE_SYSTEM;
        
        while ((c = fgetc(fres)) != '\n')
            *buffp++ = c;
        *buffp = '\0';
    }

    fclose(fres);
    memset(res->status, 0, sizeof(daemon_res_t));

    int i;
    char *proto = buff, *token;
    char *resp = (char *)res->status;
    for (i = 0; (token = strtok_r(proto, " ", &proto)); ++i) {
#ifdef __DEBUG
        printf("token: %s\n", token);
#endif
        size_t tlen = strlen(token);
        memcpy(resp, token, tlen);
        *(resp + tlen) = '\0';
        resp += daemon_res_len[i];
    }

#ifdef __DEBUG
    printf("parsed: %s %s %s\n", res->status->proto, res->status->t_code, res->status->code_text);
#endif

    return DOCKERCLIE_OK;
}

void free_daemon_response(daemon_res_t *res)
{
    if (res->status)
        free(res->status);
    if (res->res_text)
        free(res->res_text);
    if (res->data)
        free(res->data);
}
